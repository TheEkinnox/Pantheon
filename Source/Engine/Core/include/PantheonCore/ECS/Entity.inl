#pragma once
#include "PantheonCore/ECS/Entity.h"

#include "PantheonCore/Debug/Assertion.h"

namespace PantheonCore::ECS
{
    constexpr Entity::Entity(const Id id)
        : m_id(id)
    {
    }

    constexpr Entity::Entity(const Index index, const Version version)
        : Entity(make(index, version))
    {
    }

    constexpr Entity::operator Id() const
    {
        return m_id;
    }

    constexpr Entity::Version Entity::getVersion() const
    {
        return static_cast<Version>(m_id >> INDEX_BITS & VERSION_MASK);
    }

    constexpr Entity::Index Entity::getIndex() const
    {
        return static_cast<Index>(m_id & INDEX_MASK);
    }

    inline void Entity::bumpVersion()
    {
        const Version version = getVersion();
        PTH_ASSERT(getVersion() < TOMBSTONE_VERSION);
        m_id = make(getIndex(), version + 1);
    }

    constexpr Entity::Id Entity::make(const Id index, const Id version)
    {
        return (index & INDEX_MASK) | ((version & VERSION_MASK) << INDEX_BITS);
    }

    inline std::ostream& operator<<(std::ostream& stream, const Entity& entity)
    {
        return stream << entity.getIndex() << ':' << entity.getVersion();
    }
}
