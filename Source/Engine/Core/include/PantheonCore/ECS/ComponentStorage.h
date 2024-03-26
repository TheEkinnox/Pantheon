#pragma once
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/Eventing/Event.h"

#include <unordered_map>

namespace PantheonCore::ECS
{
    class EntityHandle;
    class Scene;

    class IComponentStorage
    {
    public:
        using EntitiesMap = std::unordered_map<Entity::Id, Entity>;

        /**
         * \brief Creates a copy of the given component storage
         * \param other The component storage to copy
         */
        IComponentStorage(const IComponentStorage& other) = default;

        /**
         * \brief Creates a move copy of the given component storage
         * \param other The component storage to move
         */
        IComponentStorage(IComponentStorage&& other) noexcept = default;

        /**
         * \brief Destroys the component storage
         */
        virtual ~IComponentStorage() = default;

        /**
         * \brief Assigns a copy of the given component storage to this one
         * \param other The component storage to copy
         * \return A reference to the modified component storage
         */
        IComponentStorage& operator=(const IComponentStorage& other) = default;

        /**
         * \brief Moves the given component storage into this one
         * \param other The component storage to move
         * \return A reference to the modified component storage
         */
        IComponentStorage& operator=(IComponentStorage&& other) noexcept = default;

        /**
         * \brief Checks if the given entity owns a component in the storage
         * \param entity The entity to check for
         * \return True if the entity owns a component in this storage. False otherwise
         */
        virtual bool contains(Entity entity) const = 0;

        /**
         * \brief Assigns a copy of the source entity's component to the target entity
         * \param source The entity from which the component should be copied
         * \param target The entity to which the component should be assigned
         * \return True on success. False otherwise
         */
        virtual bool copy(Entity source, Entity target) = 0;

        /**
         * \brief Removes the component owned by the given entity
         * \param owner The removed component's owner
         */
        virtual void remove(Entity owner) = 0;

        /**
         * \brief Removes all stored components
         */
        virtual void clear() = 0;

        /**
         * \brief Reserves the given number of components
         * \param count The number of components to reserve
         */
        virtual void reserve(Entity::Id count) = 0;

        /**
         * \brief Gets the current number of entities
         * \return The current number of entities
         */
        virtual Entity::Id getCount() const = 0;

        /**
         * \brief Serializes the component storage to a byte array
         * \param output The output memory buffer
         * \param entitiesMap The entity index to scene entity map
         * \return True on success. False otherwise.
         */
        virtual bool toBinary(std::vector<char>& output, const EntitiesMap& entitiesMap) const = 0;

        /**
         * \brief Deserializes the component storage from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        virtual size_t fromBinary(const char* data, size_t length) = 0;

        /**
         * \brief Serializes the component storage to json
         * \param writer The output json writer
         * \param entitiesMap The entity index to scene entity map
         * \return True on success. False otherwise.
         */
        virtual bool toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& entitiesMap) const = 0;

        /**
         * \brief Deserializes the component storage from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        virtual bool fromJson(const rapidjson::Value& json) = 0;

    protected:
        /**
         * \brief Creates a default component storage
         */
        IComponentStorage() = default;
    };

    template <class T>
    class ComponentStorage final : public IComponentStorage
    {
    public:
        using ComponentT = std::remove_const_t<T>;
        using iterator = typename std::vector<ComponentT>::iterator;
        using const_iterator = typename std::vector<ComponentT>::const_iterator;

        Eventing::Event<EntityHandle, T&> m_onAdd;
        Eventing::Event<EntityHandle, T&> m_onRemove;
        Eventing::Event<EntityHandle, T&> m_onBeforeChange;
        Eventing::Event<EntityHandle, T&> m_onChange;

        /**
         * \brief Creates an empty component storage
         * \param scene The storage's owning scene
         */
        explicit ComponentStorage(Scene* scene = nullptr);

        /**
         * \brief Creates a copy of the given component storage
         * \param other The component storage to copy
         */
        ComponentStorage(const ComponentStorage& other) = default;

        /**
         * \brief Creates a move copy of the given component storage
         * \param other The component storage to move
         */
        ComponentStorage(ComponentStorage&& other) noexcept = default;

        /**
         * \brief Destroys the component storage
         */
        ~ComponentStorage() override = default;

        /**
         * \brief Assigns a copy of the given component storage to this one
         * \param other The component storage to copy
         * \return A reference to the modified component storage
         */
        ComponentStorage& operator=(const ComponentStorage& other) = default;

        /**
         * \brief Moves the given component storage into this one
         * \param other The component storage to move
         * \return A reference to the modified component storage
         */
        ComponentStorage& operator=(ComponentStorage&& other) noexcept = default;

        /**
         * \brief Checks if the given entity owns a component in the storage
         * \param entity The entity to check for
         * \return True if the entity owns a component in this storage. False otherwise
         */
        bool contains(Entity entity) const override;

        /**
         * \brief Assigns a copy of the source entity's component to the target entity
         * \param source The entity from which the component should be copied
         * \param target The entity to which the component should be assigned
         * \return True on success. False otherwise
         */
        bool copy(Entity source, Entity target) override;

        /**
         * \brief Assigns the given component instance to the given entity
         * \param owner The component's owner
         * \param instance The added component instance
         * \return A reference to the created or modified component
         */
        T& set(Entity owner, const ComponentT& instance);

        /**
         * \brief Creates or modifies the given entity's component of the given type
         * \tparam Args The component's construction parameters
         * \param owner The component's owner
         * \param args The component's construction parameters
         * \return A reference to the created or modified component
         */
        template <typename... Args>
        T& construct(Entity owner, Args&&... args);

        /**
         * \brief Removes the component owned by the given entity
         * \param owner The removed component's owner
         */
        void remove(Entity owner) override;

        /**
         * \brief Removes the given component instance from the storage
         * \param component The component instance to remove
         */
        void remove(const T& component);

        /**
         * \brief Removes all stored components
         */
        void clear() override;

        /**
         * \brief Reserves the given number of entities
         * \param count The number of entities to reserve
         */
        void reserve(Entity::Id count) override;

        /**
         * \brief Gets the current number of entities
         * \return The current number of entities
         */
        Entity::Id getCount() const override;

        /**
         * \brief Checks if the given entity owns a component in the storage
         * \param owner The searched component's owner
         * \return True if the entity owns a component of in the storage. False otherwise
         */
        bool has(Entity owner) const;

        /**
         * \brief Finds the component owned by the given entity
         * \param owner The searched component's owner
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        T* find(Entity owner);

        /**
         * \brief Finds the component owned by the given entity
         * \param owner The searched component's owner
         * \return A constant pointer to the found component on success. Nullptr otherwise
         */
        const T* find(Entity owner) const;

        /**
         * \brief Gets the given component instance's owner
         * \param component The component who's owner to find
         * \return The component's owner on success. NULL_ENTITY otherwise
         */
        Entity getOwner(const T& component) const;

        /**
         * \brief Gets an iterator to the start of the components array
         * \return An iterator to the start of the components array
         */
        iterator begin();

        /**
         * \brief Gets an iterator to the end of the components array
         * \return An iterator to the end of the components array
         */
        iterator end();

        /**
         * \brief Gets a constant iterator to the start of the components array
         * \return A constant iterator to the start of the components array
         */
        const_iterator begin() const;

        /**
         * \brief Gets a constant iterator to the end of the components array
         * \return A constant iterator to the end of the components array
         */
        const_iterator end() const;

        /**
         * \brief Serializes the component storage to a byte array
         * \param output The output memory buffer
         * \param entitiesMap The entity index to scene entity map
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output, const EntitiesMap& entitiesMap) const override;

        /**
         * \brief Deserializes the component storage from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

        /**
         * \brief Serializes the component storage to json
         * \param writer The output json writer
         * \param entitiesMap The entity index to scene entity map
         * \return True on success. False otherwise.
         */
        bool toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& entitiesMap) const override;

        /**
         * \brief Deserializes the component storage from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool fromJson(const rapidjson::Value& json) override;

    private:
        std::vector<ComponentT>                m_components;
        std::unordered_map<Entity::Id, size_t> m_entityToComponent;
        std::unordered_map<size_t, Entity>     m_componentToEntity;
        Scene*                                 m_scene;
    };
}

#include "PantheonCore/ECS/ComponentStorage.inl"
