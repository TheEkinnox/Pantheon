#include "PantheonCore/ECS/Components/Hierarchy.h"

#include "PantheonCore/ECS/EntityHandle.h"
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

    void HierarchyComponent::setParent(const Entity parent)
    {
        m_parent = parent;
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

        const std::vector<EntityHandle> children = entity.getChildren();

        for (EntityHandle child : children)
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
            transform->setParent(parentTransform, parentTransform == nullptr);

        const std::vector<Transform*> childTransforms = getChildTransforms(entity);

        for (Transform* childTransform : childTransforms)
        {
            Transform* newParent = transform ? transform : parentTransform;
            childTransform->setParent(newParent, parentTransform == nullptr);
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
    void ComponentTraits::onAdd(EntityHandle& owner, HierarchyComponent& hierarchy)
    {
        PTH_ASSERT(hierarchy.m_firstChild == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        PTH_ASSERT(hierarchy.m_previousSibling == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        PTH_ASSERT(hierarchy.m_nextSibling == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        PTH_ASSERT(hierarchy.m_childCount == 0, "Adding a pre-existing hierarchy is not supported");

        onChange(owner, hierarchy);
    }

    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, HierarchyComponent& hierarchy)
    {
        {
            HierarchyComponent discard;
            onBeforeChange(entity, hierarchy, discard);
        }

        EntityHandle child(entity.getScene(), hierarchy.m_firstChild);

        hierarchy.m_parent          = NULL_ENTITY;
        hierarchy.m_nextSibling     = NULL_ENTITY;
        hierarchy.m_previousSibling = NULL_ENTITY;

        while (child)
        {
            const EntityHandle nextChild = child.getNextSibling();
            child.destroy();
            child = nextChild;
        }

        hierarchy.m_childCount = 0;
        unlinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, HierarchyComponent& hierarchy, HierarchyComponent&)
    {
        Scene* scene = entity.getScene();
        PTH_ASSERT(scene);

        EntityHandle parent(scene, hierarchy.m_parent);
        EntityHandle nextSibling(scene, hierarchy.m_nextSibling);
        EntityHandle prevSibling(scene, hierarchy.m_previousSibling);

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

        hierarchy.m_parent          = NULL_ENTITY;
        hierarchy.m_nextSibling     = NULL_ENTITY;
        hierarchy.m_previousSibling = NULL_ENTITY;
    }

    template <>
    void ComponentTraits::onChange(EntityHandle& entity, HierarchyComponent& hierarchy)
    {
        Scene* scene = entity.getScene();
        PTH_ASSERT(scene);

        EntityHandle parent(scene, hierarchy.m_parent);

        if (EntityHandle firstChild(scene, hierarchy.m_firstChild); firstChild && !firstChild.has<HierarchyComponent>())
            firstChild.make<HierarchyComponent>(entity);

        if (!parent)
            return;

        if (HierarchyComponent* parentHierarchy = parent.get<HierarchyComponent>())
        {
            EntityHandle nextSibling(scene, hierarchy.m_nextSibling = parentHierarchy->m_firstChild);

            if (nextSibling)
                nextSibling.get<HierarchyComponent>()->m_previousSibling = entity;

            parentHierarchy->m_firstChild = entity;
            ++parentHierarchy->m_childCount;
        }
        else
        {
            parentHierarchy               = &parent.make<HierarchyComponent>(NULL_ENTITY);
            parentHierarchy->m_firstChild = entity;
            parentHierarchy->m_childCount = 1;
        }

        linkTransforms(entity);
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
    size_t ComponentRegistry::fromBinary(HierarchyComponent& out, const char* data, size_t length, Scene*)
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
            const auto it = toSerialized.find(parent);

            if (!CHECK(it != toSerialized.end(), "Unable to serialize hierarchy component - Parent is not serialized"))
                return false;

            parent = it->second;
        }

        writer.StartObject();

        writer.Key("parent");
        writer.Uint64(parent);

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

        out.setParent(Entity(it->value.Get<Entity::Index>()));
        return true;
    }
}
