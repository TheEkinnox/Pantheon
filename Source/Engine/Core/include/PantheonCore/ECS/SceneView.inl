#pragma once
#include "PantheonCore/ECS/SceneView.h"

#include "PantheonCore/ECS/Scene.h"

namespace PantheonCore::ECS
{
    template <class... Components>
    SceneView<Components...>::SceneView(SceneRef scene)
        : m_scene(&scene)
    {
        initializeView<Components...>();
    }

    template <class... Components>
    void SceneView<Components...>::refresh()
    {
        initializeView<Components...>();
    }

    template <class... Components>
    template <typename... T>
    std::enable_if_t<(sizeof...(T) > 1), std::tuple<T*...>> SceneView<Components...>::get(const Entity owner)
    {
        return std::make_tuple(get<T>(owner)...);
    }

    template <class... Components>
    template <typename T>
    T* SceneView<Components...>::get(const Entity owner)
    {
        static_assert(Has<T>);
        return getStorage<T>().find(owner);
    }

    template <class... Components>
    std::tuple<Components*...> SceneView<Components...>::get(const Entity owner)
    {
        return get<Components...>(owner);
    }

    template <class... Components>
    template <typename T>
    ComponentStorage<T>& SceneView<Components...>::getStorage()
    {
        static_assert(Has<T>);
        if constexpr (Utility::IsOneOf<T, Components...>)
            return *std::get<IndexOf<T>>(m_storages);
        else
            return *reinterpret_cast<ComponentStorage<T>*>(std::get<IndexOf<std::remove_const_t<T>>>(m_storages));
    }

    template <class... Components>
    template <typename T>
    void SceneView<Components...>::setStorage(ComponentStorage<T>& storage)
    {
        static_assert(Has<T>);
        if constexpr (Utility::IsOneOf<T, Components...>)
            std::get<IndexOf<T>>(m_storages) = &storage;
        else
            std::get<IndexOf<std::remove_const_t<T>>>(m_storages) = &storage;
    }

    template <class... Components>
    typename SceneView<Components...>::iterator SceneView<Components...>::begin()
    {
        EntityStorageRef entities = m_scene->template getStorage<Entity>();
        return iterator(entities.begin(), entities.end(), m_storages);
    }

    template <class... Components>
    typename SceneView<Components...>::iterator SceneView<Components...>::end()
    {
        EntityStorageRef entities = m_scene->template getStorage<Entity>();
        return iterator(entities.end(), entities.end(), m_storages);
    }

    template <class... Components>
    typename SceneView<Components...>::const_iterator SceneView<Components...>::begin() const
    {
        const EntityStorage& entities = m_scene->template getStorage<Entity>();
        return const_iterator(entities.begin(), entities.end(), m_storages);
    }

    template <class... Components>
    typename SceneView<Components...>::const_iterator SceneView<Components...>::end() const
    {
        const EntityStorage& entities = m_scene->template getStorage<Entity>();
        return const_iterator(entities.end(), entities.end(), m_storages);
    }

    template <class... Components>
    template <typename T, typename... Remainder>
    void SceneView<Components...>::initializeView()
    {
        setStorage<T>(const_cast<ComponentStorage<T>&>(m_scene->template getStorage<T>()));

        if constexpr (sizeof...(Remainder) > 0)
            initializeView<Remainder...>();
    }
}
