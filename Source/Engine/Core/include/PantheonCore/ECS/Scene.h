#pragma once
#include "PantheonCore/ECS/EntityStorage.h"
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Serialization/Serializer.h"

namespace PantheonCore::ECS
{
    struct ComponentHandle;

    template <class T>
    class ComponentStorage;
    class IComponentStorage;
    class EntityHandle;

    class Scene final : public Resources::IResource
    {
        REGISTERED_RESOURCE_BODY()

    public:
        template <typename T>
        using Storage = std::conditional_t<std::is_same_v<Entity, std::remove_const_t<T>>, EntityStorage, ComponentStorage<T>>;

        /**
         * \brief Creates an empty scene
         */
        Scene();

        /**
         * \brief Creates a copy of the given scene
         * \param other The scene to copy
         */
        Scene(const Scene& other) = default;

        /**
         * \brief Creates a move copy of the given scene
         * \param other The scene to move
         */
        Scene(Scene&& other) noexcept = default;

        /**
         * \brief Destroys the scene
         */
        ~Scene() override = default;

        /**
         * \brief Assigns a copy of the given scene to this one
         * \param other The scene to copy
         * \return A reference to the modified scene
         */
        Scene& operator=(const Scene& other) = default;

        /**
         * \brief Moves the given scene into this one
         * \param other The scene to move
         * \return A reference to the modified scene
         */
        Scene& operator=(Scene&& other) noexcept = default;

        /**
         * \brief Tries to load the scene from the given file
         * \param fileName The resource file's path
         * \return True if the resource was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        /**
         * \brief Initializes the scene
         * \return True on success. False otherwise.
         */
        bool init() override
        {
            return true;
        }

        /**
         * \brief Saves the scene to the given file
         * \param fileName The target save path
         * \return True if the scene was successfully saved. False otherwise.
         */
        bool save(const std::string& fileName) const override;

        /**
         * \brief Serializes the object to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output) const override;

        /**
         * \brief Tries to load the resource from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

        /**
         * \brief Serializes the scene to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool toJson(Serialization::JsonWriter& writer) const;

        /**
         * \brief Deserializes the scene from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool fromJson(const Serialization::JsonValue& json);

        /**
         * \brief Creates a new entity
         * \return A handle to the created entity
         */
        EntityHandle create();

        /**
         * \brief Creates a copy of the given source entity
         * \param source The source entity
         * \return A handle to the created entity
         */
        EntityHandle create(Entity source);

        /**
         * \brief Finds the entity with the given index
         * \param index The entity's index
         * \return A handle to the entity with the given index on success. An empty handle otherwise
         */
        EntityHandle find(Entity::Index index);

        /**
         * \brief Destroys the given entity
         * \param entity The entity to destroy
         */
        void destroy(Entity entity);

        /**
         * \brief Checks whether the given entity is valid
         * \param entity The entity to check
         * \return True if te entity is valid. False otherwise.
         */
        bool isValid(Entity entity) const;

        /**
         * \brief Removes everything from the scene
         */
        void clear();

        /**
         * \brief Checks if the given entity is in this scene
         * \param owner The searched entity
         * \return True if the entity is in this scene. False otherwise
         */
        bool contains(Entity owner) const;

        /**
         * \brief Checks if the given entity owns a component of the given type
         * \tparam T The component's type
         * \param owner The searched component's owner
         * \return True if the entity owns a component of the given type. False otherwise
         */
        template <typename T>
        bool has(Entity owner) const;

        /**
         * \brief Finds the component owned by the given entity
         * \tparam T The component's type
         * \param owner The searched component's owner
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        T* get(Entity owner);

        /**
         * \brief Finds the component owned by the given entity
         * \tparam T The component's type
         * \param owner The searched component's owner
         * \return A constant pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        const T* get(Entity owner) const;

        /**
         * \brief Assigns the given component instance to the given entity
         * \tparam T The component's type
         * \param owner The component's owner
         * \param instance The added component instance
         * \return A reference to the created or modified component
         */
        template <typename T>
        T& set(Entity owner, const T& instance);

        /**
         * \brief Creates or modifies the given entity's component of the given type
         * \tparam T The component's type
         * \tparam Args The component's construction parameters
         * \param owner The component's owner
         * \param args The component's construction parameters
         * \return A reference to the created or modified component
         */
        template <typename T, typename... Args>
        T& make(Entity owner, Args&&... args);

        /**
         * \brief Removes the component of the given type owned by the given entity from the scene
         * \tparam T The component's type
         * \param owner The component's owner
         */
        template <typename T>
        void remove(Entity owner);

        /**
         * \brief Removes the given component instance from the scene
         * \tparam T The component's type
         * \param instance The component instance to remove
         */
        template <typename T>
        void remove(const T& instance);

        /**
         * \brief Gets the storage for the given type
         * \tparam T The storage content type
         * \return A reference to the storage
         */
        template <typename T>
        Storage<T>& getStorage();

        /**
         * \brief Gets the storage for the given type
         * \tparam T The storage's content type
         * \return A constant reference to the storage
         */
        template <typename T>
        const Storage<T>& getStorage() const;

        /**
         * \brief Gets the component storage for the given type
         * \param id The component's type's id
         * \return A reference to the storage
         */
        IComponentStorage& getStorage(Utility::TypeId id);

        /**
         * \brief Gets the component storage for the given type
         * \param id The component's type's id
         * \return A constant reference to the storage
         */
        const IComponentStorage& getStorage(Utility::TypeId id) const;

        /**
         * \brief Gets the ids of all the component types
         * \return The ids of all the component types
         */
        std::vector<Utility::TypeId> getComponentIds() const;

        /**
         * \brief Gets the number of components owned by the given entity
         * \param entity The target entity
         * \return The number of components owned by the entity
         */
        Entity::Index getComponentCount(Entity entity) const;

        /**
         * \brief Gets the ids of all the component types owned by the given entity
         * \param owner The components' owner
         * \return The ids of all the component types owned by the given entity
         */
        std::vector<Utility::TypeId> getComponentIds(Entity owner) const;

        /**
         * \brief Gets all the components owned by the given entity
         * \param owner The components' owner
         * \return The components owned by the given entity
         */
        std::vector<std::pair<Utility::TypeId, void*>> getComponents(Entity owner) const;

        /**
         * \brief Gets handles to all the components owned by the given entity
         * \param owner The components' owner
         * \return Handles to the components owned by the given entity
         */
        std::vector<ComponentHandle> getComponentHandles(Entity owner) const;

    private:
        EntityStorage                                                                   m_entities;
        mutable std::unordered_map<Utility::TypeId, std::shared_ptr<IComponentStorage>> m_components;

        /**
         * \brief Deserializes a component storage from json
         * \param json The input json data
         * \return True on success. False otherwise
         */
        bool deserializeStorage(const Serialization::JsonValue& json);

        /**
         * \brief Deserializes a component storage from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserializeStorage(const char* data, size_t length);
    };
}

#include "PantheonCore/ECS/Scene.inl"
