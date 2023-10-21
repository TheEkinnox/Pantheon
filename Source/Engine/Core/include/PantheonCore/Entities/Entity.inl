#pragma once

#include "PantheonCore/Entities/Component.h"
#include "PantheonCore/Entities/Entity.h"

namespace PantheonEngine::Core::Entities
{
    template <typename T, typename... Args>
    T& Entity::addComponent(Args&&... args)
    {
        static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

        m_components.push_back(std::make_shared<T>(*this, std::forward<Args>(args)...));

        return static_cast<T&>(*m_components.back());
    }

    template <typename T>
    void Entity::removeComponent()
    {
        static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

        const T* firstComponent = getComponent<T>();

        if (nullptr != firstComponent)
            removeComponent(*firstComponent);
    }

    template <typename T>
    T* Entity::getComponent()
    {
        static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

        for (const auto& component : m_components)
        {
            if (typeid(component.get()) == typeid(T*) || dynamic_cast<T*>(component.get()) != nullptr)
                return dynamic_cast<T*>(component.get());
        }

        return nullptr;
    }

    template <typename T>
    T* Entity::getComponent(const Component::ComponentId id)
    {
        static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

        for (const auto& component : m_components)
        {
            if (id == component->getId() &&
                (typeid(component.get()) == typeid(T*) ||
                    dynamic_cast<T*>(component.get()) != nullptr))
                return dynamic_cast<T*>(component.get());
        }

        return nullptr;
    }

    template <typename T>
    std::vector<std::shared_ptr<T>> Entity::getComponents()
    {
        static_assert(std::is_same_v<Component, T> || std::is_base_of_v<Component, T>);

        std::vector<std::shared_ptr<T>> components;

        for (const auto& component : m_components)
        {
            if (typeid(component.get()) == typeid(T*) || dynamic_cast<T*>(component.get()) != nullptr)
                components.push_back(std::dynamic_pointer_cast<T>(component));
        }

        return components;
    }
}
