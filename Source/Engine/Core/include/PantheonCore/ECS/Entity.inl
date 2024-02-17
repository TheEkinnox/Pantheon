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
}
