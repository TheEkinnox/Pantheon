#include "PantheonCore/ECS/EntityHandle.h"

#include "PantheonCore/ECS/ComponentHandle.h"
#include "PantheonCore/ECS/EntityHandleIterator.h"
#include "PantheonCore/ECS/Scene.h"
#include "PantheonCore/ECS/Components/Hierarchy.h"
#include "PantheonCore/ECS/Components/TagComponent.h"

using namespace PantheonCore::Serialization;
using namespace PantheonCore::Utility;

namespace PantheonCore::ECS
{
    EntityHandle::EntityHandle(Scene* scene, const Entity entity)
        : m_scene(scene), m_entity(entity)
    {
    }

    bool EntityHandle::operator==(const EntityHandle& other) const
    {
        return m_entity == other.m_entity && m_scene == other.m_scene;
    }

    bool EntityHandle::operator==(const Entity entity) const
    {
        return m_entity == entity;
    }

    EntityHandle::operator bool() const
    {
        return m_scene && m_scene->isValid(m_entity);
    }

    EntityHandle::operator Entity() const
    {
        return getEntity();
    }

    Entity EntityHandle::getEntity() const
    {
        return m_entity;
    }

    Scene* EntityHandle::getScene() const
    {
        return m_scene;
    }

    EntityHandle EntityHandle::getRoot() const
    {
        EntityHandle parent = *this;
        EntityHandle root;

        do
        {
            root   = parent;
            parent = parent.getParent();
        } while (parent);

        return root;
    }

    EntityHandle EntityHandle::getParent() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return { m_scene, hierarchy ? hierarchy->getParent() : NULL_ENTITY };
    }

    std::string EntityHandle::getDisplayName() const
    {
        if (!*this)
            return "None";

        std::ostringstream str;
        if (const TagComponent* tag = get<TagComponent>(); tag && !tag->m_tag.empty())
            str << tag->m_tag << " (" << m_entity << ")";
        else
            str << "Entity " << m_entity;

        return str.str();
    }

    void EntityHandle::setParent(const EntityHandle parent)
    {
        HierarchyComponent* hierarchy = get<HierarchyComponent>();

        if (!hierarchy)
        {
            make<HierarchyComponent>(parent);
            return;
        }

        hierarchy->setParent(parent);
        set<HierarchyComponent>(*hierarchy);
    }

    EntityHandle EntityHandle::getNextSibling() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return { m_scene, hierarchy ? hierarchy->getNextSibling() : NULL_ENTITY };
    }

    EntityHandle EntityHandle::getPreviousSibling() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return { m_scene, hierarchy ? hierarchy->getPreviousSibling() : NULL_ENTITY };
    }

    Entity::Index EntityHandle::getChildCount() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        return hierarchy ? hierarchy->getChildCount() : 0;
    }

    EntityHandle EntityHandle::getChild(Entity::Index index) const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        if (!hierarchy || hierarchy->getChildCount() <= index)
            return { m_scene, NULL_ENTITY };

        EntityHandle child = { m_scene, hierarchy->getFirstChild() };

        while (index-- > 0)
            child = child.getNextSibling();

        return child;
    }

    EntityHandle::iterator EntityHandle::begin() const
    {
        return { getChild(0) };
    }

    EntityHandle::iterator EntityHandle::end() const
    {
        return { { m_scene, NULL_ENTITY } };
    }

    EntityHandle::reverse_iterator EntityHandle::rbegin() const
    {
        return { getChild(getChildCount() - 1) };
    }

    EntityHandle::reverse_iterator EntityHandle::rend() const
    {
        return { { m_scene, NULL_ENTITY } };
    }

    std::vector<EntityHandle> EntityHandle::getChildren() const
    {
        return { begin(), end() };
    }

    EntityHandle EntityHandle::copy() const
    {
        if (!*this)
            return {};

        return m_scene->create(m_entity);
    }

    void EntityHandle::destroy()
    {
        if (m_scene)
            m_scene->destroy(m_entity);

        m_entity = NULL_ENTITY;
    }

    bool EntityHandle::has(const TypeId type) const
    {
        if (!m_scene)
            return false;

        return m_scene->getStorage(type).contains(m_entity);
    }

    ComponentHandle EntityHandle::get(const TypeId type) const
    {
        if (m_scene)
            return { *this, type };

        return {};
    }

    ComponentHandle EntityHandle::getInParent(const TypeId type) const
    {
        ComponentHandle current{ *this, type };

        if (current)
            return current;

        EntityHandle parent = getParent();

        while (parent)
        {
            if ((current = { parent, type }))
                return current;

            parent = parent.getParent();
        }

        return {};
    }

    ComponentHandle EntityHandle::getInChildren(const TypeId type) const
    {
        if (const ComponentHandle component = get(type))
            return component;

        for (const EntityHandle& child : *this)
        {
            if (const ComponentHandle component = child.getInChildren(type))
                return component;
        }

        return {};
    }

    ComponentHandle EntityHandle::getInHierarchy(const TypeId type, const EComponentSearchOrigin searchOrigin) const
    {
        switch (searchOrigin)
        {
        case EComponentSearchOrigin::ROOT:
        {
            return getRoot().getInChildren(type);
        }
        case EComponentSearchOrigin::PARENT:
        {
            if (const ComponentHandle component = getInParent(type))
                return component;

            return getInChildren(type);
        }
        case EComponentSearchOrigin::CHILDREN:
        {
            if (const ComponentHandle component = getInChildren(type))
                return component;

            return getInParent(type);
        }
        default:
            PTH_ASSERT(false, "Invalid component search origin");
            return {};
        }
    }

    ComponentHandle EntityHandle::getOrCreate(const TypeId type) const
    {
        if (!m_scene)
            return {};

        if (m_scene->getStorage(type).getOrCreateRaw(m_entity))
            return { *this, type };

        return {};
    }

    void EntityHandle::remove(const TypeId type) const
    {
        if (m_scene)
            m_scene->getStorage(type).remove(m_entity);
    }

    Entity::Id EntityHandle::getComponentCount() const
    {
        return m_scene ? m_scene->getComponentCount(m_entity) : 0;
    }

    std::vector<TypeId> EntityHandle::getComponentIds() const
    {
        return m_scene ? m_scene->getComponentIds(m_entity) : std::vector<TypeId>();
    }

    std::vector<std::pair<TypeId, void*>> EntityHandle::getComponents() const
    {
        return m_scene ? m_scene->getComponents(m_entity) : std::vector<std::pair<TypeId, void*>>();
    }

    std::vector<ComponentHandle> EntityHandle::getComponentHandles() const
    {
        return m_scene ? m_scene->getComponentHandles(m_entity) : std::vector<ComponentHandle>{};
    }

    std::ostream& operator<<(std::ostream& stream, const EntityHandle& handle)
    {
        return stream << handle.getEntity();
    }

    template <>
    bool ComponentRegistry::toBinary(const EntityHandle& component, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        Entity entity = component.getEntity();

        if (entity != NULL_ENTITY)
        {
            const auto it = toSerialized.find(entity.getIndex());

            if (!CHECK(it != toSerialized.end(), "Unable to serialize entity handle - Entity is not serialized"))
                return false;

            entity = it->second;
        }

        return CHECK(IByteSerializable::writeNumber(entity.getIndex(), out), "Failed to write entity handle's entity");
    }

    template <>
    size_t ComponentRegistry::fromBinary(EntityHandle& out, const char* data, size_t length, Scene* scene)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize entity handle - Empty buffer"))
            return 0;

        Entity entity;

        const size_t readBytes = IByteSerializable::readNumber<Entity, Entity::Index>(entity, data, length);

        if (!CHECK(readBytes != 0, "Failed to read entity handle's entity"))
            return 0;

        out = { scene, entity };
        return readBytes;
    }

    template <>
    bool ComponentRegistry::toJson(const EntityHandle& component, JsonWriter& writer, const EntitiesMap& toSerialized)
    {
        Entity entity = component.getEntity();

        if (entity != NULL_ENTITY)
        {
            const auto it = toSerialized.find(entity.getIndex());

            if (!CHECK(it != toSerialized.end(), "Unable to serialize entity handle - Entity is not serialized"))
                return false;

            entity = it->second;
        }

        return writer.Uint64(entity.getIndex());
    }

    template <>
    bool ComponentRegistry::fromJson(EntityHandle& out, const JsonValue& json, Scene* scene)
    {
        if (!CHECK(json.Is<Entity::Index>(), "Unable to deserialize entity handle - Json value should be castable to Entity::Index"))
            return false;

        out = { scene, Entity(json.Get<Entity::Index>()) };
        return true;
    }
}
