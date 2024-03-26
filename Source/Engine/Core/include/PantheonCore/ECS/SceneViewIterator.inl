#pragma once
#include "PantheonCore/ECS/SceneViewIterator.h"

namespace PantheonCore::ECS
{
    template <bool IsConst, class... Components>
    SceneViewIterator<IsConst, Components...>::SceneViewIterator(
        iterator_type current, iterator_type end, const StorageTuple& storages)
        : m_storages(&storages), m_iterator(current), m_end(end)
    {
        while (m_iterator != m_end && !isValid(*m_iterator))
            ++m_iterator;
    }

    template <bool IsConst, class... Components>
    bool SceneViewIterator<IsConst, Components...>::operator==(const SceneViewIterator& other) const
    {
        return m_iterator == other.m_iterator;
    }

    template <bool IsConst, class... Components>
    bool SceneViewIterator<IsConst, Components...>::operator!=(const SceneViewIterator& other) const
    {
        return !(*this == other);
    }

    template <bool IsConst, class... Components>
    typename SceneViewIterator<IsConst, Components...>::reference SceneViewIterator<IsConst, Components...>::operator*() const
    {
        return *m_iterator;
    }

    template <bool IsConst, class... Components>
    typename SceneViewIterator<IsConst, Components...>::pointer SceneViewIterator<IsConst, Components...>::operator->() const
    {
        return &operator*();
    }

    template <bool IsConst, class... Components>
    SceneViewIterator<IsConst, Components...>& SceneViewIterator<IsConst, Components...>::operator++()
    {
        while (++m_iterator != m_end && !isValid(*m_iterator))
        {
            // Do nothing
        }

        return *this;
    }

    template <bool IsConst, class... Components>
    SceneViewIterator<IsConst, Components...> SceneViewIterator<IsConst, Components...>::operator++(int)
    {
        SceneViewIterator tmp = *this;
        ++(*this);
        return tmp;
    }

    template <bool IsConst, class... Components>
    template <size_t Index>
    bool SceneViewIterator<IsConst, Components...>::isValid(const value_type entity)
    {
        if (entity == NULL_ENTITY)
            return false;

        const auto* storage = std::get<Index>(*m_storages);

        if constexpr (Index == sizeof...(Components) - 1)
            return storage && storage->contains(entity);
        else
            return storage && storage->contains(entity) && isValid<Index + 1>(entity);
    }
}
