#pragma once
#include "PantheonCore/ECS/EntityHandle.h"

namespace PantheonCore::ECS
{
    template <bool IsReverse>
    class EntityHandleIterator
    {
    public:
        using iterator_category = std::bidirectional_iterator_tag;
        using value_type        = EntityHandle;
        using difference_type   = Utility::SmallestIntT<Entity::Index>;
        using pointer           = value_type*;
        using reference         = value_type&;

        /**
         * \brief Creates a default entity handle iterator
         */
        EntityHandleIterator() = default;

        /**
         * \brief Creates an iterator for the given entity handle
         * \param entity The iterated entity
         */
        EntityHandleIterator(EntityHandle entity);

        /**
         * \brief Creates a copy of the given entity handle iterator
         * \param other The entity handle iterator to copy
         */
        EntityHandleIterator(const EntityHandleIterator& other) = default;

        /**
         * \brief Creates a move copy of the given entity handle iterator
         * \param other The entity handle iterator to move
         */
        EntityHandleIterator(EntityHandleIterator&& other) noexcept = default;

        /**
         * \brief Destroys the entity handle iterator
         */
        ~EntityHandleIterator() = default;

        /**
         * \brief Assigns a copy of the given entity handle iterator to this one
         * \param other The entity handle iterator to copy
         * \return A reference to the modified entity handle iterator
         */
        EntityHandleIterator& operator=(const EntityHandleIterator& other) = default;

        /**
         * \brief Moves the given entity handle iterator into this one
         * \param other The entity handle iterator to move
         * \return A reference to the modified entity handle iterator
         */
        EntityHandleIterator& operator=(EntityHandleIterator&& other) noexcept = default;

        /**
         * \brief Checks if the given iterator references the same entity as this one
         * \param other The entity handle iterator to compare against
         * \return True if the iterators reference the same entity. False otherwise
         */
        bool operator==(const EntityHandleIterator& other) const;

        /**
         * \brief Dereferences the entity handle iterator
         * \return A reference to the iterated entity
         */
        reference operator*() const;

        /**
         * \brief Dereferences the entity handle iterator
         * \return A pointer to the iterated entity
         */
        pointer operator->() const;

        /**
         * \brief Pre-increments the entity handle iterator
         * \return A reference to the incremented iterator
         */
        EntityHandleIterator& operator++();

        /**
         * \brief Post-increments the entity handle iterator
         * \return A copy of the iterator before the increment
         */
        EntityHandleIterator operator++(int);

        /**
         * \brief Pre-decrements the entity handle iterator
         * \return A reference to the decremented iterator
         */
        EntityHandleIterator& operator--();

        /**
         * \brief Post-decrements the entity handle iterator
         * \return A copy of the iterator before the decrement
         */
        EntityHandleIterator operator--(int);

    private:
        EntityHandle m_entity;
    };
}

#include "PantheonCore/ECS/EntityHandleIterator.inl"
