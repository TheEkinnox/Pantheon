#include "PantheonCore/ECS/EntityHandle.h"

#include "PantheonCore/ECS/Scene.h"
#include "PantheonCore/ECS/Components/Hierarchy.h"

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

    std::vector<EntityHandle> EntityHandle::getChildren() const
    {
        const HierarchyComponent* hierarchy = get<HierarchyComponent>();

        if (!hierarchy || hierarchy->getChildCount() == 0)
            return {};

        std::vector<EntityHandle> children;
        EntityHandle              child = { m_scene, hierarchy->getFirstChild() };

        while (child)
        {
            children.push_back(child);
            child = child.getNextSibling();
        }

        return children;
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
