#pragma once
#include "PantheonCore/ECS/EntityHandle.h"
#include "PantheonCore/ECS/Scene.h"

namespace PantheonCore::ECS
{
    template <typename T>
    bool EntityHandle::has() const
    {
        return m_scene ? m_scene->has<T>(m_entity) : false;
    }

    template <typename T>
    T* EntityHandle::get()
    {
        return m_scene ? m_scene->get<T>(m_entity) : nullptr;
    }

    template <typename T>
    const T* EntityHandle::get() const
    {
        return m_scene ? static_cast<const Scene*>(m_scene)->get<T>(m_entity) : nullptr;
    }

    template <typename T>
    T* EntityHandle::getInParent()
    {
        if (T* component = get<T>())
            return component;

        EntityHandle parent = getParent();

        while (parent)
        {
            if (T* component = parent.getInParent<T>())
                return component;

            parent = parent.getParent();
        }

        return nullptr;
    }

    template <typename T>
    const T* EntityHandle::getInParent() const
    {
        if (const T* component = get<T>())
            return component;

        EntityHandle parent = getParent();

        while (parent)
        {
            if (const T* component = parent.getInParent<T>())
                return component;

            parent = parent.getParent();
        }

        return nullptr;
    }

    template <typename T>
    T* EntityHandle::getInChildren()
    {
        if (T* component = get<T>())
            return component;

        const std::vector<EntityHandle> children = getChildren();

        for (const EntityHandle& child : children)
        {
            if (T* component = child.getInChildren<T>())
                return component;
        }

        return nullptr;
    }

    template <typename T>
    const T* EntityHandle::getInChildren() const
    {
        if (const T* component = get<T>())
            return component;

        const std::vector<EntityHandle> children = getChildren();

        for (const EntityHandle& child : children)
        {
            if (const T* component = child.getInChildren<T>())
                return component;
        }

        return nullptr;
    }

    template <typename T>
    T* EntityHandle::getInHierarchy(const EComponentSearchOrigin searchOrigin)
    {
        switch (searchOrigin)
        {
        case EComponentSearchOrigin::ROOT:
        {
            return getRoot().getInChildren<T>();
        }
        case EComponentSearchOrigin::PARENT:
        {
            if (T* component = getInParent<T>())
                return component;

            return getInChildren<T>();
        }
        case EComponentSearchOrigin::CHILDREN:
        {
            if (T* component = getInChildren<T>())
                return component;

            return getInParent<T>();
        }
        default:
            ASSERT(false, "Invalid component search origin");
            return nullptr;
        }
    }

    template <typename T>
    const T* EntityHandle::getInHierarchy(const EComponentSearchOrigin searchOrigin) const
    {
        switch (searchOrigin)
        {
        case EComponentSearchOrigin::ROOT:
        {
            return getRoot().getInChildren<T>();
        }
        case EComponentSearchOrigin::PARENT:
        {
            if (const T* component = getInParent<T>())
                return component;

            return getInChildren<T>();
        }
        case EComponentSearchOrigin::CHILDREN:
        {
            if (const T* component = getInChildren<T>())
                return component;

            return getInParent<T>();
        }
        default:
            ASSERT(false, "Invalid component search origin");
            return nullptr;
        }
    }

    template <typename T>
    T& EntityHandle::set(const T& instance)
    {
        ASSERT(*this);
        return m_scene->set<T>(m_entity, instance);
    }

    template <typename T, typename... Args>
    T& EntityHandle::make(Args&&... args)
    {
        ASSERT(*this);
        return m_scene->make<T>(m_entity, std::forward<Args>(args)...);
    }

    template <typename T>
    void EntityHandle::remove()
    {
        if (m_scene)
            m_scene->remove<T>(m_entity);
    }

    template <typename T>
    void EntityHandle::remove(const T& instance)
    {
        if (m_scene)
            m_scene->remove<T>(instance);
    }
}
