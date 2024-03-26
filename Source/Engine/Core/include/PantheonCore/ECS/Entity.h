#pragma once
#include <climits>
#include <cstdint>

namespace PantheonCore::ECS
{
    class Entity
    {
    public:
        using Id = uint64_t;
        using Version = uint32_t;

        static constexpr uint8_t VERSION_BITS = 24;
        static constexpr uint8_t INDEX_BITS   = sizeof(Id) * CHAR_BIT - VERSION_BITS;

        static constexpr Version VERSION_MASK = (Version{ 1 } << VERSION_BITS) - 1;
        static constexpr Id      INDEX_MASK   = (Id{ 1 } << INDEX_BITS) - 1;

        /**
         * \brief Creates a default entity
         */
        constexpr Entity() = default;

        /**
         * \brief Creates an entity with the given id
         * \param id The entity's id
         */
        explicit constexpr Entity(Id id);

        /**
         * \brief Creates an entity with the given index and version
         * \param index The entity's index
         * \param version The entity's version
         */
        constexpr Entity(Id index, Version version);

        /**
         * \brief Implicitly converts an entity to it's index
         */
        constexpr operator Id() const;

        /**
         * \brief Gets the entity's version
         * \return The entity's version
         */
        constexpr Version getVersion() const;

        /**
         * \brief Gets the entity's index
         * \return The entity's index
         */
        constexpr Id getIndex() const;

        /**
         * \brief Increments the entity's version
         */
        void bumpVersion();

    private:
        Id m_id = 0;

        /**
         * \brief Creates an entity identifier from the given index and version
         * \param index The target index
         * \param version The target version
         * \return The entity identifier composed of the given index and version
         */
        static constexpr Id make(Id index, Id version);
    };
}

#include "PantheonCore/ECS/Entity.inl"

namespace PantheonCore::ECS
{
    static constexpr Entity NULL_ENTITY{ Entity::INDEX_MASK, Entity::VERSION_MASK };
}
