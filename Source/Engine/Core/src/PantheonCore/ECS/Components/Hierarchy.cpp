#include "PantheonCore/ECS/Components/Hierarchy.h"

#include "PantheonCore/ECS/EntityHandle.h"
#include "PantheonCore/ECS/EntityHandleIterator.h"
#include "PantheonCore/ECS/SceneView.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    HierarchyComponent::HierarchyComponent(const Entity parent)
        : m_parent(parent)
    {
    }

    Entity HierarchyComponent::getParent() const
    {
        return m_parent;
    }

    Entity HierarchyComponent::getFirstChild() const
    {
        return m_firstChild;
    }

    Entity HierarchyComponent::getPreviousSibling() const
    {
        return m_previousSibling;
    }

    Entity HierarchyComponent::getNextSibling() const
    {
        return m_nextSibling;
    }

    Entity::Index HierarchyComponent::getChildCount() const
    {
        return m_childCount;
    }

    std::vector<Transform*> getChildTransforms(const EntityHandle& entity)
    {
        std::vector<Transform*> linkedTransforms;

        for (EntityHandle child : entity)
        {
            if (Transform* childTransform = child.get<Transform>())
            {
                linkedTransforms.push_back(childTransform);
            }
            else
            {
                const std::vector<Transform*> childTransforms = getChildTransforms(child);
                linkedTransforms.insert(linkedTransforms.end(), childTransforms.begin(), childTransforms.end());
            }
        }

        return linkedTransforms;
    }

    void linkTransforms(EntityHandle& entity)
    {
        Transform* parentTransform = entity.getParent().getInParent<Transform>();
        Transform* transform       = entity.get<Transform>();

        if (transform)
            transform->setParent(parentTransform, !parentTransform);

        const std::vector<Transform*> childTransforms = getChildTransforms(entity);

        for (Transform* childTransform : childTransforms)
        {
            Transform* newParent = transform ? transform : parentTransform;
            childTransform->setParent(newParent, !newParent);
        }
    }

    void unlinkTransforms(const EntityHandle& entity)
    {
        Transform* parentTransform = entity.getParent().getInParent<Transform>();

        const std::vector<Transform*> linkedTransforms = getChildTransforms(entity);

        for (Transform* transform : linkedTransforms)
            transform->setParent(parentTransform, true);
    }

    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, HierarchyComponent& component)
    {
        PTH_ASSERT(component.m_firstChild == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        PTH_ASSERT(component.m_previousSibling == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        PTH_ASSERT(component.m_nextSibling == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        PTH_ASSERT(component.m_childCount == 0, "Adding a pre-existing hierarchy is not supported");

        onChange(entity, component);
    }

    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, HierarchyComponent& component)
    {
        {
            HierarchyComponent discard;
            onBeforeChange(entity, component, discard);
        }

        EntityHandle child(entity.getScene(), component.m_firstChild);

        // Technically the component gets deleted but events, which get called after this, might need the info to be accurate
        component.m_parent          = NULL_ENTITY;
        component.m_nextSibling     = NULL_ENTITY;
        component.m_previousSibling = NULL_ENTITY;

        while (child)
        {
            const EntityHandle nextChild = child.getNextSibling();
            child.destroy();
            child = nextChild;
        }

        component.m_childCount = 0;
        unlinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, HierarchyComponent& component, HierarchyComponent& newValue)
    {
        Scene* scene = entity.getScene();
        PTH_ASSERT(scene);

        if (newValue.m_parent == entity)
            newValue.m_parent = component.m_parent;

        newValue.m_firstChild = component.m_firstChild;
        newValue.m_childCount = component.m_childCount;

        EntityHandle parent(scene, component.m_parent);
        EntityHandle nextSibling(scene, component.m_nextSibling);
        EntityHandle prevSibling(scene, component.m_previousSibling);

        if (HierarchyComponent* parentHierarchy = parent.get<HierarchyComponent>())
        {
            --parentHierarchy->m_childCount;

            if (entity.getEntity() == parentHierarchy->m_firstChild)
                parentHierarchy->m_firstChild = nextSibling;
        }

        if (HierarchyComponent* prevHierarchy = prevSibling.get<HierarchyComponent>())
            prevHierarchy->m_nextSibling = nextSibling;

        if (HierarchyComponent* nextHierarchy = nextSibling.get<HierarchyComponent>())
            nextHierarchy->m_previousSibling = prevSibling;

        // Technically the component gets overwritten but events, which get called after this, might need the info to be accurate
        component.m_parent          = NULL_ENTITY;
        component.m_nextSibling     = newValue.m_nextSibling     = NULL_ENTITY;
        component.m_previousSibling = newValue.m_previousSibling = NULL_ENTITY;
    }

    template <>
    void ComponentTraits::onChange(EntityHandle& entity, HierarchyComponent& component)
    {
        Scene* scene = entity.getScene();
        PTH_ASSERT(scene);

        if (component.m_parent == entity)
            component.m_parent = NULL_ENTITY;

        EntityHandle parent(scene, component.m_parent);
        EntityHandle firstChild(scene, component.m_firstChild);

        if (HierarchyComponent* parentHierarchy = parent.get<HierarchyComponent>())
        {
            component.m_nextSibling = parentHierarchy->m_firstChild;
            EntityHandle nextSibling(scene, component.m_nextSibling);

            if (nextSibling)
                nextSibling.get<HierarchyComponent>()->m_previousSibling = entity;

            parentHierarchy->m_firstChild = entity;
            ++parentHierarchy->m_childCount;
        }
        else if (parent)
        {
            parentHierarchy               = &parent.make<HierarchyComponent>(NULL_ENTITY);
            parentHierarchy->m_firstChild = entity;
            parentHierarchy->m_childCount = 1;
        }

        if (firstChild && !firstChild.has<HierarchyComponent>())
            firstChild.make<HierarchyComponent>(entity);

        linkTransforms(entity);
    }

    template <>
    HierarchyComponent ComponentTraits::copy(EntityHandle&, HierarchyComponent& source, EntityHandle& to)
    {
        PTH_ASSERT(to);
        Scene* scene = to.getScene();

        to.setParent({ scene, source.m_parent }, true);

        const EntityHandle firstChild(scene, source.m_firstChild);
        EntityHandle       child = firstChild;

        while (child)
        {
            const EntityHandle nextSibling = child.getNextSibling();

            EntityHandle copy = child.copy();
            copy.setParent(to, true);

            child = nextSibling;
        }

        return *to.get<HierarchyComponent>();
    }

    template <>
    void ComponentTraits::onAdd<Transform>(EntityHandle& entity, Transform&)
    {
        linkTransforms(entity);
    }

    template <>
    void ComponentTraits::onRemove<Transform>(EntityHandle& entity, Transform&)
    {
        unlinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onChange<Transform>(EntityHandle& entity, Transform&)
    {
        linkTransforms(entity);
    }

    template <>
    bool ComponentRegistry::toBinary(const HierarchyComponent& hierarchy, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        Entity parent = hierarchy.getParent();

        if (parent != NULL_ENTITY)
        {
            const auto it = toSerialized.find(parent.getIndex());

            if (!CHECK(it != toSerialized.end(), "Unable to serialize hierarchy component - Parent is not serialized"))
                return false;

            parent = it->second;
        }

        return CHECK(IByteSerializable::writeNumber(parent.getIndex(), out), "Unable to serialize hierarchy's parent");
    }

    template <>
    size_t ComponentRegistry::fromBinary(HierarchyComponent& out, const char* data, const size_t length, Scene*)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize hierarchy - Empty buffer"))
            return 0;

        return IByteSerializable::readNumber<Entity, Entity::Index>(out.m_parent, data, length);
    }

    template <>
    bool ComponentRegistry::toJson(const HierarchyComponent& hierarchy, JsonWriter& writer, const EntitiesMap& toSerialized)
    {
        Entity parent = hierarchy.getParent();

        if (parent != NULL_ENTITY)
        {
            const auto it = toSerialized.find(parent.getIndex());

            if (!CHECK(it != toSerialized.end(), "Unable to serialize hierarchy component - Parent is not serialized"))
                return false;

            parent = it->second;
        }

        writer.StartObject();

        writer.Key("parent");
        if constexpr (sizeof(Entity::Index) <= sizeof(uint32_t))
            writer.Uint(parent.getIndex());
        else
            writer.Uint64(parent.getIndex());

        return writer.EndObject();
    }

    template <>
    bool ComponentRegistry::fromJson(HierarchyComponent& out, const JsonValue& json, Scene*)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize hierarchy - Json value should be an object"))
            return false;

        const auto it = json.FindMember("parent");
        if (!CHECK(it != json.MemberEnd() && it->value.Is<Entity::Index>(), "Unable to deserialize hierarchy - Invalid parent"))
            return false;

        out.m_parent = Entity(it->value.Get<Entity::Index>());
        return true;
    }
}
