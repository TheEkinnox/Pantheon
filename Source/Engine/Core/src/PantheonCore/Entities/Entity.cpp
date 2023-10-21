#include "PantheonCore/Entities/Entity.h"

namespace PantheonEngine::Core::Entities
{
    Entity::Entity(Entity* parent, const Transform& transform) :
        Node(parent), Transform(transform)
    {
    }

    Entity::Entity(const Entity& other) :
        Node(other), Transform(other),
        m_components(other.m_components)
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = *this;
    }

    Entity::Entity(Entity&& other) noexcept :
        Node(std::move(other)), Transform(std::move(other)),
        m_components(std::move(other.m_components))
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = *this;
    }

    Entity::~Entity()
    {
        if (!m_components.empty())
            m_components.clear();
    }

    Entity& Entity::operator=(const Entity& other)
    {
        if (&other == this)
            return *this;

        Node::operator=(other);
        Transform::operator=(other);

        m_components.clear();

        m_components = other.m_components;

        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = *this;

        return *this;
    }

    Entity& Entity::operator=(Entity&& other) noexcept
    {
        if (&other == this)
            return *this;

        Node::operator=(other);
        Transform::operator=(other);

        m_components.clear();

        m_components = std::move(other.m_components);

        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = *this;

        return *this;
    }

    void Entity::removeComponent(const Component& component)
    {
        if (m_components.empty())
            return;

        const auto findFunc = [component](const ComponentPtr& ptr)
        {
            return *ptr == component;
        };

        std::erase_if(m_components, findFunc);
    }

    void Entity::removeComponent(const Component::ComponentId id)
    {
        if (m_components.empty())
            return;

        const auto findFunc = [id](const ComponentPtr& ptr)
        {
            return ptr->getId() == id;
        };

        std::erase_if(m_components, findFunc);
    }

    void Entity::update()
    {
        for (const auto& component : m_components)
            component->update();

        for (Node* child : getChildren())
            reinterpret_cast<Entity*>(child)->update();
    }

    void Entity::addChild(Node& child)
    {
        Node::addChild(child);
        reinterpret_cast<Entity&>(child).setParent(*this);
    }

    void Entity::removeChild(Node& child)
    {
        Node::removeChild(child);
        reinterpret_cast<Entity&>(child).removeParent();
    }
}
