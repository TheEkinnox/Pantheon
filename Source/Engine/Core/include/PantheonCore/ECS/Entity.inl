#pragma once
#include "PantheonCore/ECS/Entity.h"

#include "PantheonCore/Debug/Assertion.h"

namespace PantheonCore::ECS
{
    constexpr Entity::Entity(const Id id)
        : m_id(id)
    {
    }

    constexpr Entity::Entity(const Id index, const Version version)
        : Entity(make(index, version))
    {
    }

    constexpr Entity::operator Id() const
    {
        return m_id;
    }

    constexpr Entity::Version Entity::getVersion() const
    {
        return m_id >> INDEX_BITS & VERSION_MASK;
    }

    constexpr Entity::Id Entity::getIndex() const
    {
        return m_id & INDEX_MASK;
    }

    inline void Entity::bumpVersion()
    {
        const Version version = getVersion();
        ASSERT(version < VERSION_MASK);
        m_id = make(getIndex(), version + 1);
    }

    constexpr Entity::Id Entity::make(const Id index, const Id version)
    {
        return (index & INDEX_MASK) | ((version & VERSION_MASK) << INDEX_BITS);
    }
}
