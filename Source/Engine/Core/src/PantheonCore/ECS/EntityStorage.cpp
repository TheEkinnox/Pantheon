#include "PantheonCore/ECS/EntityStorage.h"
#include "PantheonCore/ECS/EntityHandle.h"
#include "PantheonCore/ECS/Scene.h"

namespace PantheonCore::ECS
{
    EntityStorage::EntityStorage(Scene* scene)
        : m_scene(scene)
    {
    }

    Entity EntityStorage::add()
    {
        if (m_entities.size() == m_count)
            return m_entities.emplace_back(m_count++, Entity::Version{});

        Entity& entity = m_entities[m_count++];
        entity.bumpVersion();

        m_onAdd.invoke({ m_scene, entity });
        return entity;
    }

    void EntityStorage::remove(const Entity entity)
    {
        const auto it = std::ranges::find(*this, entity);

        if (it == end())
            return;

        m_onRemove.invoke({ m_scene, *it });

        std::swap(*it, m_entities[m_count - 1]);
        --m_count;
    }

    bool EntityStorage::has(const Entity entity) const
    {
        return std::ranges::find(*this, entity) != end();
    }

    void EntityStorage::clear()
    {
        m_entities.clear();
        m_count = 0;
    }

    void EntityStorage::reserve(const size_t count)
    {
        m_entities.reserve(count);
    }

    EntityStorage::iterator EntityStorage::begin()
    {
        return m_entities.begin();
    }

    EntityStorage::iterator EntityStorage::end()
    {
        return m_entities.end() - static_cast<iterator::difference_type>(m_entities.size() - m_count);
    }

    EntityStorage::const_iterator EntityStorage::begin() const
    {
        return m_entities.begin();
    }

    EntityStorage::const_iterator EntityStorage::end() const
    {
        return m_entities.end() - static_cast<const_iterator::difference_type>(m_entities.size() - m_count);
    }

    Entity::Id EntityStorage::getCount() const
    {
        return m_count;
    }
}
