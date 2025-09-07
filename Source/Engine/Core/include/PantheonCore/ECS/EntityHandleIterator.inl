#pragma once
#include "PantheonCore/ECS/EntityHandleIterator.h"

namespace PantheonCore::ECS
{
    template <bool IsReverse>
    EntityHandleIterator<IsReverse>::EntityHandleIterator(const EntityHandle entity)
        : m_entity(entity)
    {
    }

    template <bool IsReverse>
    bool EntityHandleIterator<IsReverse>::operator==(const EntityHandleIterator& other) const
    {
        return other.m_entity == m_entity;
    }

    template <bool IsReverse>
    typename EntityHandleIterator<IsReverse>::reference EntityHandleIterator<IsReverse>::operator*() const
    {
        return const_cast<reference>(m_entity);
    }

    template <bool IsReverse>
    typename EntityHandleIterator<IsReverse>::pointer EntityHandleIterator<IsReverse>::operator->() const
    {
        return const_cast<pointer>(&m_entity);
    }

    template <bool IsReverse>
    EntityHandleIterator<IsReverse>& EntityHandleIterator<IsReverse>::operator++()
    {
        if constexpr (IsReverse)
            m_entity = m_entity.getPreviousSibling();
        else
            m_entity = m_entity.getNextSibling();

        return *this;
    }

    template <bool IsReverse>
    EntityHandleIterator<IsReverse> EntityHandleIterator<IsReverse>::operator++(int)
    {
        EntityHandle tmp = m_entity;
        return (++(*this), tmp);
    }

    template <bool IsReverse>
    EntityHandleIterator<IsReverse>& EntityHandleIterator<IsReverse>::operator--()
    {
        if constexpr (IsReverse)
            m_entity = m_entity.getNextSibling();
        else
            m_entity = m_entity.getPreviousSibling();

        return *this;
    }

    template <bool IsReverse>
    EntityHandleIterator<IsReverse> EntityHandleIterator<IsReverse>::operator--(int)
    {
        EntityHandle tmp = m_entity;
        return (--(*this), tmp);
    }
}
