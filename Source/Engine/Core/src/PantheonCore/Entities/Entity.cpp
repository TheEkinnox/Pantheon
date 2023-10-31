#include "PantheonCore/Entities/Entity.h"

namespace PantheonEngine::Core::Entities
{
    Entity::Entity(Entity* parent, const Transform& transform) :
        Node(parent), Transform(transform), m_isActive(true), m_isDestroyed(false)
    {
    }

    Entity::Entity(const Entity& other) :
        Node(other), Transform(other), m_components(other.m_components), m_isActive(other.m_isActive), m_isDestroyed(false)
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;
    }

    Entity::Entity(Entity&& other) noexcept :
        Node(std::forward<Node&&>(other)), Transform(std::forward<Transform&&>(other)),
        m_components(std::move(other.m_components)), m_isActive(other.m_isActive), m_isDestroyed(false)
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;
    }

    Entity::~Entity()
    {
        m_isDestroyed = true;

        if (!m_components.empty())
            m_components.clear();
    }

    Entity::operator bool() const
    {
        return !m_isDestroyed;
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
                component->m_owner = this;

        m_isActive = other.m_isActive;

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
                component->m_owner = this;

        m_isActive = other.m_isActive;

        return *this;
    }

    void Entity::removeComponent(const Component& component)
    {
        if (m_components.empty())
            return;

        const auto findFunc = [&component](const ComponentPtr& ptr)
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
        if (!isActive())
            return;

        for (const auto& component : m_components)
        {
            if (component->isActive())
                component->update();
        }

        for (NodePtr& child : getChildren())
            reinterpret_cast<Entity&>(*child).update();
    }

    bool Entity::isActive() const
    {
        const auto* parent = reinterpret_cast<const Entity*>(Node::getParent());
        return !m_isDestroyed && m_isActive && (parent == nullptr || parent->isActive());
    }

    void Entity::setActive(bool active)
    {
        m_isActive = active;
    }

    void Entity::onChildAdded(Node& child)
    {
        reinterpret_cast<Entity&>(child).setParent(*this);
    }

    void Entity::onRemoveChild(Node& child)
    {
        reinterpret_cast<Entity&>(child).removeParent();
    }
}
