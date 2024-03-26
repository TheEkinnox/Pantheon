#pragma once
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/Eventing/Event.h"

namespace PantheonCore::ECS
{
    class EntityHandle;
    class Scene;

    class EntityStorage
    {
    public:
        using iterator = std::vector<Entity>::iterator;
        using const_iterator = std::vector<Entity>::const_iterator;

        Eventing::Event<EntityHandle> m_onAdd;
        Eventing::Event<EntityHandle> m_onRemove;

        /**
         * \brief Creates an empty entity storage
         * \param scene The storage's owning scene
         */
        explicit EntityStorage(Scene* scene = nullptr);

        /**
         * \brief Creates a copy of the given entity storage
         * \param other The entity storage to copy
         */
        EntityStorage(const EntityStorage& other) = default;

        /**
         * \brief Creates a move copy of the given entity storage
         * \param other The entity storage to move
         */
        EntityStorage(EntityStorage&& other) noexcept = default;

        /**
         * \brief Destroys the entity storage
         */
        ~EntityStorage() = default;

        /**
         * \brief Assigns a copy of the given entity storage to this one
         * \param other The entity storage to copy
         * \return A reference to the modified entity storage
         */
        EntityStorage& operator=(const EntityStorage& other) = default;

        /**
         * \brief Moves the given entity storage into this one
         * \param other The entity storage to move
         * \return A reference to the modified entity storage
         */
        EntityStorage& operator=(EntityStorage&& other) noexcept = default;

        /**
         * \brief Adds an entity to the manager
         * \return The added entity
         */
        Entity add();

        /**
         * \brief Removes the given entity from the manager
         * \param entity The entity to remove
         */
        void remove(Entity entity);

        /**
         * \brief Checks whether the given entity is present in the manager or not
         * \param entity The entity to search for
         * \return True if the entity was found. False otherwise
         */
        bool has(Entity entity) const;

        /**
         * \brief Removes all entities from the manager
         */
        void clear();

        /**
         * \brief Reserves the given number of entities
         * \param count The number of entities to reserve
         */
        void reserve(size_t count);

        /**
         * \brief Gets an iterator to the start of the entities array
         * \return An iterator to the start of the entities array
         */
        iterator begin();

        /**
         * \brief Gets an iterator to the end of the entities array
         * \return An iterator to the end of the entities array
         */
        iterator end();

        /**
         * \brief Gets a constant iterator to the start of the entities array
         * \return A constant iterator to the start of the entities array
         */
        const_iterator begin() const;

        /**
         * \brief Gets a constant iterator to the end of the entities array
         * \return A constant iterator to the end of the entities array
         */
        const_iterator end() const;

        /**
         * \brief Gets the current number of entities
         * \return The current number of entities
         */
        Entity::Id getCount() const;

    private:
        std::vector<Entity> m_entities;
        Entity::Id          m_count = 0;
        Scene*              m_scene = nullptr;
    };
}
