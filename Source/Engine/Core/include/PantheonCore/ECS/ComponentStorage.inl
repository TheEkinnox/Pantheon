#pragma once
#include "PantheonCore/ECS/ComponentStorage.h"

#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentTraits.h"
#include "PantheonCore/ECS/EntityHandle.h"
#include "PantheonCore/Serialization/IByteSerializable.h"

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
    T& ComponentStorage<T>::set(const Entity owner, ComponentT instance)
    {
        return construct(owner, std::move(instance));
    }

    template <class T>
    template <typename... Args>
    T& ComponentStorage<T>::construct(const Entity owner, Args&&... args)
    {
        const auto   it = m_entityToComponent.find(owner);
        EntityHandle handle(m_scene, owner);

        if (it != m_entityToComponent.end())
        {
            ComponentT& component = m_components[it->second];
            ComponentT newVal(std::forward<Args>(args)...);

            ComponentTraits::onBeforeChange<ComponentT>(handle, component, newVal);
            m_onBeforeChange.invoke(handle, component, newVal);

            component = *new(&component) ComponentT(std::move(newVal));

            ComponentTraits::onChange<ComponentT>(handle, component);
            m_onChange.invoke(handle, component);
            return component;
        }

        ComponentT&  component = m_components.emplace_back(std::forward<Args>(args)...);
        const auto index     = static_cast<Entity::Index>(m_components.size() - 1);

        m_componentToEntity[index] = owner;
        m_entityToComponent[owner] = index;

        ComponentTraits::onAdd<ComponentT>(handle, component);
        m_onAdd.invoke(handle, component);

        return component;
    }

    template <class T>
    void ComponentStorage<T>::remove(const Entity owner)
    {
        const auto it = m_entityToComponent.find(owner);

        if (it == m_entityToComponent.end())
            return;

        ComponentT&  component = m_components[it->second];
        EntityHandle handle(m_scene, owner);

        ComponentTraits::onRemove<ComponentT>(handle, component);
        m_onRemove.invoke(handle, component);

        const Entity::Index lastIndex = m_components.size() - 1;

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
        for (Entity::Index i = m_components.size(); i > 0; --i)
        {
            if (i > m_components.size())
                continue;

            EntityHandle handle(m_scene, m_componentToEntity[i - 1]);

            if (!handle)
                continue;

            ComponentT& component = m_components[i - 1];

            ComponentTraits::onRemove<ComponentT>(handle, component);
            m_onRemove.invoke(handle, component);
        }

        m_components.clear();
        m_componentToEntity.clear();
        m_entityToComponent.clear();
    }

    template <class T>
    void ComponentStorage<T>::reserve(const Entity::Index count)
    {
        m_components.reserve(count);
    }

    template <class T>
    Entity::Index ComponentStorage<T>::size() const
    {
        return static_cast<Entity::Index>(m_components.size());
    }

    template <class T>
    void* ComponentStorage<T>::getOrCreateRaw(const Entity owner)
    {
        void* out = findRaw(owner);
        return out ? out : (void*)&construct(owner);
    }

    template <class T>
    void* ComponentStorage<T>::findRaw(const Entity owner)
    {
        return (void*)find(owner);
    }

    template <class T>
    const void* ComponentStorage<T>::findRaw(const Entity owner) const
    {
        return (const void*)find(owner);
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
        return const_cast<ComponentStorage*>(this)->find(owner);
    }

    template <class T>
    Entity ComponentStorage<T>::getOwner(const T& component) const
    {
        Entity::Index index = 0;

        for (const auto& current : m_components)
        {
            if (&component == &current)
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
        if (!CHECK(Serialization::IByteSerializable::writeNumber(size(), output), "Failed to write component storage size"))
            return false;

        output.reserve(output.size() + size() * (sizeof(T) + sizeof(Entity::Id)));

        for (const auto [index, entity] : m_componentToEntity)
        {
            const auto it = entitiesMap.find(entity.getIndex());

            if (!CHECK(it != entitiesMap.end(), "Failed to serialize component storage - Entity %" ENT_IDX_FMT " not found", entity.getIndex()))
                return false;

            if (!CHECK(Serialization::IByteSerializable::writeNumber(it->second, output), "Failed to write component owner"))
                return false;

            if (!ComponentRegistry::toBinary(m_components[index], output, entitiesMap))
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

        Entity::Index count;
        size_t        offset = IByteSerializable::readNumber(count, data, length);

        if (!CHECK(offset > 0, "Failed to read component storage size"))
            return 0;

        reserve(count);

        for (size_t i = 0; i < count; ++i)
        {
            Entity entity    = NULL_ENTITY;
            size_t readBytes = length >= offset ? IByteSerializable::readNumber<Entity, Entity::Index>(entity, data + offset, length - offset) : 0;

            if (!CHECK(readBytes > 0, "Failed to read component owner"))
                return false;

            offset += readBytes;
            ComponentT component;
            readBytes = length >= offset ? ComponentRegistry::fromBinary(component, data + offset, length - offset, m_scene) : 0;

            if (readBytes == 0)
                return false;

            set(entity, component);
            offset += readBytes;
        }

        return offset;
    }

    template <class T>
    bool ComponentStorage<T>::toJson(Serialization::JsonWriter& writer, const EntitiesMap& entitiesMap) const
    {
        writer.StartArray();

        for (const auto [index, entity] : m_componentToEntity)
        {
            const auto it = entitiesMap.find(entity.getIndex());

            if (!CHECK(it != entitiesMap.end(), "Failed to serialize component storage - Entity %" ENT_IDX_FMT " not found", entity.getIndex()))
                return false;

            writer.StartObject();

            writer.Key("owner");
            if (!CHECK(writer.Uint64(it->second), "Failed to write component owner"))
                return false;

            writer.Key("data");
            if (!ComponentRegistry::toJson(m_components[index], writer, entitiesMap))
                return false;

            writer.EndObject();
        }

        return writer.EndArray();
    }

    template <class T>
    bool ComponentStorage<T>::fromJson(const Serialization::JsonValue& json)
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
            if (!ComponentRegistry::fromJson(component, it->value, m_scene))
                return false;

            set(owner, component);
        }

        return true;
    }
}
