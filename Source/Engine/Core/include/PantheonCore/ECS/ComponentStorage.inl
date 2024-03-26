#pragma once
#include "PantheonCore/ECS/ComponentStorage.h"

#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentTraits.h"

namespace PantheonCore::ECS
{
    template <class T>
    ComponentStorage<T>::ComponentStorage(Scene* scene)
        : m_scene(scene)
    {
    }

    template <class T>
    bool ComponentStorage<T>::contains(const Entity entity) const
    {
        return m_entityToComponent.contains(entity);
    }

    template <class T>
    bool ComponentStorage<T>::copy(const Entity source, const Entity target)
    {
        const auto sourceIt = m_entityToComponent.find(source);

        if (sourceIt == m_entityToComponent.end())
            return false;

        set(target, m_components[sourceIt->second]);
        return true;
    }

    template <class T>
    T& ComponentStorage<T>::set(const Entity owner, const ComponentT& instance)
    {
        const auto it = m_entityToComponent.find(owner);

        if (it != m_entityToComponent.end())
        {
            ComponentT& component = m_components[it->second];

            ComponentTraits::onBeforeChange<ComponentT>({ m_scene, owner }, component);
            m_onBeforeChange.invoke({ m_scene, owner }, component);

            component = instance;

            ComponentTraits::onChange<ComponentT>({ m_scene, owner }, component);
            m_onChange.invoke({ m_scene, owner }, component);

            return component;
        }

        ComponentT&  component = m_components.emplace_back(instance);
        const size_t index     = m_components.size() - 1;

        m_componentToEntity[index] = owner;
        m_entityToComponent[owner] = index;

        ComponentTraits::onAdd<ComponentT>({ m_scene, owner }, component);
        m_onAdd.invoke({ m_scene, owner }, component);

        return component;
    }

    template <class T>
    template <typename... Args>
    T& ComponentStorage<T>::construct(const Entity owner, Args&&... args)
    {
        const auto it = m_entityToComponent.find(owner);

        if (it != m_entityToComponent.end())
        {
            ComponentT& component = m_components[it->second];

            ComponentTraits::onBeforeChange<ComponentT>({ m_scene, owner }, component);
            m_onBeforeChange.invoke({ m_scene, owner }, component);

            component = *new(&component) ComponentT(std::forward<Args>(args)...);

            ComponentTraits::onChange<ComponentT>({ m_scene, owner }, component);
            m_onChange.invoke({ m_scene, owner }, component);
            return component;
        }

        ComponentT&  component = m_components.emplace_back(std::forward<Args>(args)...);
        const size_t index     = m_components.size() - 1;

        m_componentToEntity[index] = owner;
        m_entityToComponent[owner] = index;

        ComponentTraits::onAdd<ComponentT>({ m_scene, owner }, component);
        m_onAdd.invoke({ m_scene, owner }, component);

        return component;
    }

    template <class T>
    void ComponentStorage<T>::remove(const Entity owner)
    {
        const auto it = m_entityToComponent.find(owner);

        if (it == m_entityToComponent.end())
            return;

        ComponentT& component = m_components[it->second];

        ComponentTraits::onRemove<ComponentT>({ m_scene, owner }, component);
        m_onRemove.invoke({ m_scene, owner }, component);

        const size_t lastIndex = m_components.size() - 1;

        m_componentToEntity[it->second] = m_componentToEntity[lastIndex];
        std::swap(component, m_components[lastIndex]);
        m_entityToComponent[m_componentToEntity[it->second]] = it->second;

        m_componentToEntity.erase(lastIndex);
        m_components.resize(lastIndex);
        m_entityToComponent.erase(it);
    }

    template <class T>
    void ComponentStorage<T>::remove(const T& component)
    {
        remove(getOwner(component));
    }

    template <class T>
    void ComponentStorage<T>::clear()
    {
        m_components.clear();
        m_componentToEntity.clear();
        m_entityToComponent.clear();
    }

    template <class T>
    void ComponentStorage<T>::reserve(const Entity::Id count)
    {
        m_components.reserve(count);
    }

    template <class T>
    Entity::Id ComponentStorage<T>::getCount() const
    {
        return static_cast<Entity::Id>(m_components.size());
    }

    template <class T>
    bool ComponentStorage<T>::has(const Entity owner) const
    {
        return m_entityToComponent.contains(owner);
    }

    template <class T>
    T* ComponentStorage<T>::find(const Entity owner)
    {
        const auto it = m_entityToComponent.find(owner);
        return it != m_entityToComponent.end() ? &m_components[it->second] : nullptr;
    }

    template <class T>
    const T* ComponentStorage<T>::find(const Entity owner) const
    {
        const auto it = m_entityToComponent.find(owner);
        return it != m_entityToComponent.end() ? &m_components[it->second] : nullptr;
    }

    template <class T>
    Entity ComponentStorage<T>::getOwner(const T& component) const
    {
        size_t index = 0;

        while (index < m_components.size())
        {
            if (&m_components[index] == &component)
                break;

            ++index;
        }

        const auto it = m_componentToEntity.find(index);
        return it != m_componentToEntity.end() ? it->second : NULL_ENTITY;
    }

    template <class T>
    typename ComponentStorage<T>::iterator ComponentStorage<T>::begin()
    {
        return m_components.begin();
    }

    template <class T>
    typename ComponentStorage<T>::iterator ComponentStorage<T>::end()
    {
        return m_components.end();
    }

    template <class T>
    typename ComponentStorage<T>::const_iterator ComponentStorage<T>::begin() const
    {
        return m_components.begin();
    }

    template <class T>
    typename ComponentStorage<T>::const_iterator ComponentStorage<T>::end() const
    {
        return m_components.end();
    }

    template <class T>
    bool ComponentStorage<T>::toBinary(std::vector<char>& output, const EntitiesMap& entitiesMap) const
    {
        if (!CHECK(Serialization::IByteSerializable::writeNumber(getCount(), output), "Failed to write component storage size"))
            return false;

        output.reserve(output.size() + getCount() * (sizeof(T) + sizeof(Entity::Id)));

        for (const auto [index, entity] : m_componentToEntity)
        {
            const auto it = entitiesMap.find(entity);

            if (!CHECK(it != entitiesMap.end(), "Failed to serialize component storage - Entity %d not found", entity.getIndex()))
                return false;

            if (!CHECK(Serialization::IByteSerializable::writeNumber(it->second, output), "Failed to write component owner"))
                return false;

            if (!ComponentRegistry::toBinary(&m_components[index], output, entitiesMap))
                return false;
        }

        return true;
    }

    template <class T>
    size_t ComponentStorage<T>::fromBinary(const char* data, size_t length)
    {
        using Serialization::IByteSerializable;

        if (!CHECK(data != nullptr && length > 0, "Failed to deserialize component storage - Empty buffer"))
            return 0;

        Entity::Id count;
        size_t     offset = IByteSerializable::readNumber(count, data, length);

        if (!CHECK(offset > 0, "Failed to read component storage size"))
            return 0;

        reserve(count);

        for (size_t i = 0; i < count; ++i)
        {
            Entity::Id id        = NULL_ENTITY;
            size_t     readBytes = length >= offset ? IByteSerializable::readNumber(id, data + offset, length - offset) : 0;

            if (!CHECK(readBytes > 0, "Failed to read component owner"))
                return false;

            offset += readBytes;
            ComponentT component;
            readBytes = length >= offset ? ComponentRegistry::fromBinary(&component, data + offset, length - offset) : 0;

            if (readBytes == 0)
                return false;

            set(Entity(id), component);
            offset += readBytes;
        }

        return offset;
    }

    template <class T>
    bool ComponentStorage<T>::toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& entitiesMap) const
    {
        writer.StartArray();

        for (const auto [index, entity] : m_componentToEntity)
        {
            const auto it = entitiesMap.find(entity);

            if (!CHECK(it != entitiesMap.end(), "Failed to serialize component storage - Entity %d not found", entity.getIndex()))
                return false;

            writer.StartObject();

            writer.Key("owner");
            if (!CHECK(writer.Uint64(it->second), "Failed to write component owner"))
                return false;

            writer.Key("data");
            if (!ComponentRegistry::toJson(&m_components[index], writer, entitiesMap))
                return false;

            writer.EndObject();
        }

        return writer.EndArray();
    }

    template <class T>
    bool ComponentStorage<T>::fromJson(const rapidjson::Value& json)
    {
        if (!CHECK(json.IsArray(), "Failed to deserialize component storage - Json value should be an array"))
            return false;

        for (const auto& jsonComponent : json.GetArray())
        {
            if (!CHECK(jsonComponent.IsObject(), "Failed to deserialize storage component - Json value should be an object"))
                return false;

            auto it = jsonComponent.FindMember("owner");

            if (!CHECK(it != jsonComponent.MemberEnd() && it->value.Is<Entity::Id>(), "Failed to read component owner"))
                return false;

            Entity owner(it->value.Get<Entity::Id>());

            it = jsonComponent.FindMember("data");

            if (!CHECK(it != jsonComponent.MemberEnd(), "Failed to read component"))
                return false;

            ComponentT component;
            if (!ComponentRegistry::fromJson(&component, it->value))
                return false;

            set(owner, component);
        }

        return true;
    }
}
