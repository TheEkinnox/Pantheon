#pragma once
#include "PantheonCore/ECS/Scene.h"

#include "PantheonCore/ECS/ComponentStorage.h"

namespace PantheonCore::ECS
{
    template <typename T>
    bool Scene::has(Entity owner) const
    {
        const auto it = m_components.find(typeid(T).hash_code());

        if (it == m_components.end())
            return false;

        return reinterpret_cast<ComponentStorage<T>*>(it->second.get())->has(owner);
    }

    template <typename T>
    T* Scene::get(Entity owner)
    {
        const auto it = m_components.find(typeid(T).hash_code());

        if (it == m_components.end())
            return nullptr;

        return reinterpret_cast<ComponentStorage<T>*>(it->second.get())->find(owner);
    }

    template <typename T>
    const T* Scene::get(Entity owner) const
    {
        const auto it = m_components.find(typeid(T).hash_code());

        if (it == m_components.end())
            return nullptr;

        return reinterpret_cast<const ComponentStorage<T>*>(it->second.get())->find(owner);
    }

    template <typename T>
    T& Scene::set(Entity owner, const T& instance)
    {
        return getStorage<T>().set(owner, instance);
    }

    template <typename T, typename... Args>
    T& Scene::make(Entity owner, Args&&... args)
    {
        return getStorage<T>().construct(owner, std::forward<Args>(args)...);
    }

    template <typename T>
    void Scene::remove(Entity owner)
    {
        getStorage<T>().remove(owner);
    }

    template <typename T>
    void Scene::remove(const T& instance)
    {
        getStorage<T>().remove(instance);
    }

    template <typename T>
    Scene::Storage<T>& Scene::getStorage()
    {
        if constexpr (std::is_same_v<Entity, T>)
        {
            return m_entities;
        }
        else
        {
            const TypeId typeHash = typeid(T).hash_code();
            const auto   it       = m_components.find(typeHash);

            if (it != m_components.end())
                return reinterpret_cast<ComponentStorage<T>&>(*it->second);

            auto& storage = *(m_components[typeHash] = std::make_unique<ComponentStorage<T>>(this));
            return reinterpret_cast<ComponentStorage<T>&>(storage);
        }
    }

    template <typename T>
    const Scene::Storage<T>& Scene::getStorage() const
    {
        if constexpr (std::is_same_v<Entity, T>)
        {
            return m_entities;
        }
        else
        {
            const TypeId typeHash = typeid(T).hash_code();
            const auto   it       = m_components.find(typeHash);

            if (it != m_components.end())
                return reinterpret_cast<const ComponentStorage<T>&>(*it->second);

            const auto& storage = *(m_components[typeHash] = std::make_unique<ComponentStorage<T>>(const_cast<Scene*>(this)));
            return reinterpret_cast<const ComponentStorage<T>&>(storage);
        }
    }
}
