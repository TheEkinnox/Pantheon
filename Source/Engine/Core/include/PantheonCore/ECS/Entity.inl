#pragma once
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/ECS/Component.h"

namespace PantheonCore::ECS
{
    template <typename T, typename... Args>
    T* Entity::addComponent(Args&&... args)
    {
        static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

        T* component = createComponent<T>(*this, std::forward<Args>(args)...);
        component->onAdd();

        return reinterpret_cast<T*>(m_components.emplace_back(component));
    }

    template <typename T>
    void Entity::removeComponent()
    {
        if (const T* firstComponent = getComponent<T>())
            removeComponent(firstComponent->getId());
    }

    template <typename T>
    T* Entity::getComponent()
    {
        for (Component* component : m_components)
        {
            if (T* foundComponent = dynamic_cast<T*>(component))
                return foundComponent;
        }

        return nullptr;
    }

    template <typename T>
    const T* Entity::getComponent() const
    {
        for (const Component* component : m_components)
        {
            if (const T* foundComponent = dynamic_cast<const T*>(component))
                return foundComponent;
        }

        return nullptr;
    }

    template <typename T>
    T* Entity::getComponent(const Component::ComponentId id)
    {
        for (const auto& component : m_components)
        {
            if (id != component->getId())
                continue;

            if (T* foundComponent = dynamic_cast<T*>(component))
                return foundComponent;
        }

        return nullptr;
    }

    template <typename T>
    const T* Entity::getComponent(const Component::ComponentId id) const
    {
        for (const auto& component : m_components)
        {
            if (id != component->getId())
                continue;

            if (const T* foundComponent = component)
                return foundComponent;
        }

        return nullptr;
    }

    template <typename T>
    std::vector<T*> Entity::getComponents()
    {
        std::vector<T*> components;

        for (const auto& component : m_components)
        {
            if (T* foundComponent = dynamic_cast<T*>(component))
                components.push_back(foundComponent);
        }

        return components;
    }

    template <typename T>
    std::vector<const T*> Entity::getComponents() const
    {
        std::vector<const T*> components;

        for (const auto& component : m_components)
        {
            if (const T* foundComponent = dynamic_cast<const T*>(component))
                components.push_back(foundComponent);
        }

        return components;
    }

    template <typename T>
    T* Entity::getComponentInParent(const bool includeInactive)
    {
        if (T* component = getComponent<T>())
            return component;

        Entity* parent = static_cast<Entity*>(getParent());

        if (!parent || !includeInactive && !parent->isActive())
            return nullptr;

        return parent->getComponentInParent<T>(includeInactive);
    }

    template <typename T>
    const T* Entity::getComponentInParent(const bool includeInactive) const
    {
        if (T* component = getComponent<T>())
            return component;

        const Entity* parent = static_cast<const Entity*>(getParent());

        if (!parent || !includeInactive && !parent->isActive())
            return nullptr;

        return parent->getComponentInParent<T>(includeInactive);
    }

    template <typename T>
    T* Entity::getComponentInParent(const Component::ComponentId id, const bool includeInactive)
    {
        if (T* component = getComponent<T>(id))
            return component;

        Entity* parent = static_cast<Entity*>(getParent());

        if (!parent || !includeInactive && !parent->isActive())
            return nullptr;

        return parent->getComponentInParent<T>(id, includeInactive);
    }

    template <typename T>
    const T* Entity::getComponentInParent(const Component::ComponentId id, const bool includeInactive) const
    {
        if (T* component = getComponent<T>(id))
            return component;

        const Entity* parent = static_cast<const Entity*>(getParent());

        if (!parent || !includeInactive && !parent->isActive())
            return nullptr;

        return parent->getComponentInParent<T>(id, includeInactive);
    }

    template <typename T>
    std::vector<T*> Entity::getComponentsInParent(const bool includeInactive)
    {
        std::vector<T*> components(getComponents<T>());

        Entity* parent = static_cast<Entity*>(getParent());

        if (parent && (includeInactive || parent->isActive()))
            components.insert(components.end(), parent->getComponentsInParent<T>(includeInactive));

        return components;
    }

    template <typename T>
    std::vector<const T*> Entity::getComponentsInParent(const bool includeInactive) const
    {
        std::vector<const T*> components(getComponents<T>());

        const Entity* parent = static_cast<const Entity*>(getParent());

        if (parent && (includeInactive || parent->isActive()))
            components.insert(components.end(), parent->getComponentsInParent<T>(includeInactive));

        return components;
    }

    template <typename T>
    T* Entity::getComponentInChildren(const bool includeInactive)
    {
        T* component = getComponent<T>();

        if (component)
            return component;

        for (const auto& child : getChildren())
        {
            Entity& childEntity = reinterpret_cast<Entity&>(*child);

            if (!includeInactive && !childEntity.isActive())
                continue;

            component = childEntity.getComponentInChildren<T>(includeInactive);

            if (component)
                return component;
        }

        return nullptr;
    }

    template <typename T>
    const T* Entity::getComponentInChildren(const bool includeInactive) const
    {
        const T* component = getComponent<T>();

        if (component)
            return component;

        for (const auto& child : getChildren())
        {
            const Entity& childEntity = reinterpret_cast<const Entity&>(*child);

            if (!includeInactive && !childEntity.isActive())
                continue;

            component = childEntity.getComponentInChildren<T>(includeInactive);

            if (component)
                return component;
        }

        return nullptr;
    }

    template <typename T>
    T* Entity::getComponentInChildren(const Component::ComponentId id, const bool includeInactive)
    {
        T* component = getComponent<T>(id);

        if (component)
            return component;

        for (const auto& child : getChildren())
        {
            Entity& childEntity = reinterpret_cast<Entity&>(*child);

            if (!includeInactive && !childEntity.isActive())
                continue;

            component = childEntity.getComponentInChildren<T>(id, includeInactive);

            if (component)
                return component;
        }

        return nullptr;
    }

    template <typename T>
    const T* Entity::getComponentInChildren(const Component::ComponentId id, const bool includeInactive) const
    {
        const T* component = getComponent<T>(id);

        if (component)
            return component;

        for (const auto& child : getChildren())
        {
            const Entity& childEntity = reinterpret_cast<const Entity&>(*child);

            if (!includeInactive && !childEntity.isActive())
                continue;

            component = childEntity.getComponentInChildren<T>(id, includeInactive);

            if (component)
                return component;
        }

        return nullptr;
    }

    template <typename T>
    std::vector<T*> Entity::getComponentsInChildren(const bool includeInactive)
    {
        std::vector<T*> components(getComponents<T>());

        for (const auto& child : getChildren())
        {
            Entity& childEntity = reinterpret_cast<Entity&>(*child);

            if (includeInactive || childEntity.isActive())
                components.insert(components.end(), childEntity.getComponentsInChildren<T>(includeInactive));
        }

        return components;
    }

    template <typename T>
    std::vector<const T*> Entity::getComponentsInChildren(const bool includeInactive) const
    {
        std::vector<const T*> components(getComponents<T>());

        for (const auto& child : getChildren())
        {
            const Entity& childEntity = reinterpret_cast<const Entity&>(*child);

            if (includeInactive || childEntity.isActive())
                components.insert(components.end(), childEntity.getComponentsInChildren<T>(includeInactive));
        }

        return components;
    }
}
