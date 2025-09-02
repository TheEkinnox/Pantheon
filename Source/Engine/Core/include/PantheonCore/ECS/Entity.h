#pragma once
#include "PantheonCore/Utility/TypeTraits.h"

#include <climits>
#include <cstdint>
#include <ostream>

namespace PantheonCore::ECS
{
    class Entity
    {
    public:
        using Id = uint64_t;

        static constexpr uint8_t VERSION_BITS = 32;
        static constexpr uint8_t INDEX_BITS   = sizeof(Id) * CHAR_BIT - VERSION_BITS;

        using Version = Utility::SmallestUInt<VERSION_BITS>;
        using Index   = Utility::SmallestUInt<INDEX_BITS>;

        static constexpr Version VERSION_MASK = (~Version{ 0 }) >> (sizeof(Version) * CHAR_BIT - VERSION_BITS);
        static constexpr Index   INDEX_MASK   = (~Index{ 0 }) >> (sizeof(Index) * CHAR_BIT - INDEX_BITS);

        static constexpr Version TOMBSTONE_VERSION = VERSION_MASK;
        static constexpr Index   TOMBSTONE_INDEX   = INDEX_MASK;

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
        constexpr Entity(Index index, Version version);

        /**
         * \brief Implicitly converts an entity to it's id
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
        constexpr Index getIndex() const;

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

    /**
     * \brief Adds an entity's string representation to the given output stream
     * \param stream The output stream
     * \param entity The output entity
     * \return The modified stream
     */
    std::ostream& operator<<(std::ostream& stream, const Entity& entity);
}

#include "PantheonCore/ECS/Entity.inl"

namespace PantheonCore::ECS
{
    static constexpr Entity NULL_ENTITY{ Entity::TOMBSTONE_INDEX, Entity::TOMBSTONE_VERSION };
}
