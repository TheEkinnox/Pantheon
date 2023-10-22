#include "PantheonCore/Entities/Component.h"

#include "PantheonCore/Entities/Entity.h"

namespace PantheonEngine::Core::Entities
{
    Component::Component(const Component& other) :
        m_owner(other.m_owner), m_id(s_currentId++), m_isActive(other.m_isActive)
    {
    }

    Component::Component(Component&& other) noexcept :
        m_owner(other.m_owner), m_id(other.m_id), m_isActive(other.m_isActive)
    {
        other.m_id = 0;
    }

    Component& Component::operator=(const Component& other)
    {
        if (&other == this)
            return *this;

        m_owner = other.m_owner;
        m_isActive = other.m_isActive;
        m_id = s_currentId++;

        return *this;
    }

    Component& Component::operator=(Component&& other) noexcept
    {
        if (&other == this)
            return *this;

        m_owner = other.m_owner;
        m_isActive = other.m_isActive;
        m_id = other.m_id;

        other.m_id = 0;

        return *this;
    }

    bool Component::operator==(const Component& other) const
    {
        return m_id == other.m_id;
    }

    bool Component::operator!=(const Component& other) const
    {
        return !(*this == other);
    }

    Component::~Component()
    {
        if (*m_owner)
            m_owner->removeComponent(m_id);
    }

    bool Component::isActive() const
    {
        return m_isActive && m_owner->isActive();
    }

    void Component::setActive(const bool active)
    {
        if (active == m_isActive)
            return;

        m_isActive = active;

        m_isActive ? onEnable() : onDisable();
    }

    Component::ComponentId Component::getId() const
    {
        return m_id;
    }

    Entity& Component::getOwner() const
    {
        return *m_owner;
    }

    Component::Component(Entity& owner) :
        m_owner(&owner), m_id(s_currentId++)
    {
    }
}
