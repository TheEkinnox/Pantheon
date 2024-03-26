#pragma once
#include "PantheonCore/ECS/Entity.h"

namespace PantheonCore::ECS
{
    class Scene;

    class EntityHandle final
    {
    public:
        enum class EComponentSearchOrigin
        {
            ROOT,
            PARENT,
            CHILDREN
        };

        /**
         * \brief Creates a handle for the given entity
         * \param scene The linked entity's scene
         * \param entity The linked entity
         */
        EntityHandle(Scene* scene = nullptr, Entity entity = NULL_ENTITY);

        /**
         * \brief Creates a copy of the given entity handle
         * \param other The entity handle to copy
         */
        EntityHandle(const EntityHandle& other) = default;

        /**
         * \brief Creates a move copy of the given entity handle
         * \param other The entity handle to move
         */
        EntityHandle(EntityHandle&& other) noexcept = default;

        /**
         * \brief Destroys the entity handle
         */
        ~EntityHandle() = default;

        /**
         * \brief Assigns a copy of the given entity handle to this one
         * \param other The entity handle to copy
         * \return A reference to the modified handle
         */
        EntityHandle& operator=(const EntityHandle& other) = default;

        /**
         * \brief Moves the given entity handle into this one
         * \param other The entity handle to move
         * \return A reference to the modified handle
         */
        EntityHandle& operator=(EntityHandle&& other) noexcept = default;

        /**
         * \brief Checks whether the entity is valid or not
         * \return True if the entity is valid. False otherwise.
         */
        explicit operator bool() const;

        /**
         * \brief Implicitly converts the handle to it's linked entity
         */
        operator Entity() const;

        /**
         * \brief Gets the handle's linked entity
         * \return The handle's linked entity
         */
        Entity getEntity() const;

        /**
         * \brief Gets the entity's scene
         * \return A pointer to the entity's scene
         */
        Scene* getScene() const;

        /**
         * \brief Gets the root of the entity's hierarchy
         * \return A handle to the root of the entity's hierarchy
         */
        EntityHandle getRoot() const;

        /**
         * \brief Gets the entity's parent
         * \return A handle to the entity's parent if found or to NULL_ENTITY otherwise
         */
        EntityHandle getParent() const;

        /**
         * \brief Sets the linked entity's parent
         * \param parent The entity's new parent
         */
        void setParent(EntityHandle parent);

        /**
         * \brief Gets the entity's next sibling
         * \return A handle to the entity's next sibling if found or to NULL_ENTITY otherwise
         */
        EntityHandle getNextSibling() const;

        /**
         * \brief Gets the entity's previous sibling
         * \return A handle to the entity's previous sibling if found or to NULL_ENTITY otherwise
         */
        EntityHandle getPreviousSibling() const;

        /**
         * \brief Gets the entity's child count
         * \return The entity's child count
         */
        size_t getChildCount() const;

        /**
         * \brief Gets the entity's child at the given index
         * \param index The target child's index
         * \return A handle to the child if found or to NULL_ENTITY otherwise
         */
        EntityHandle getChild(size_t index) const;

        /**
         * \brief Gets the entity's children
         * \return A vector of the entity's children
         */
        std::vector<EntityHandle> getChildren() const;

        /**
         * \brief Creates a copy of the linked component
         * \return A handle to the entity's copy
         */
        EntityHandle copy() const;

        /**
         * \brief Destroys the linked entity
         */
        void destroy();

        /**
         * \brief Checks if the linked entity owns a component of the given type
         * \tparam T The component's type
         * \return True if the entity owns a component of the given type. False otherwise
         */
        template <typename T>
        bool has() const;

        /**
         * \brief Finds the component owned by the linked entity
         * \tparam T The component's type
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        T* get();

        /**
         * \brief Finds the component owned by the linked entity
         * \tparam T The component's type
         * \return A constant pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        const T* get() const;

        /**
         * \brief Finds the component of the given type owned by the entity or one of it's parents
         * \tparam T The component's type
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        T* getInParent();

        /**
         * \brief Finds the component of the given type owned by the entity or one of it's parents
         * \tparam T The component's type
         * \return A constant pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        const T* getInParent() const;

        /**
         * \brief Finds the component of the given type owned by the entity or one of it's children
         * \tparam T The component's type
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        T* getInChildren();

        /**
         * \brief Finds the component of the given type owned by the entity or one of it's children
         * \tparam T The component's type
         * \return A constant pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        const T* getInChildren() const;

        /**
         * \brief Finds the component of the given type owned by an entity in the linked entity's hierarchy
         * \tparam T The component's type
         * \param searchOrigin The point at which the search should start
         * \return A pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        T* getInHierarchy(EComponentSearchOrigin searchOrigin);

        /**
         * \brief Finds the component of the given type owned by an entity in the linked entity's hierarchy
         * \tparam T The component's type
         * \param searchOrigin The point at which the search should start
         * \return A constant pointer to the found component on success. Nullptr otherwise
         */
        template <typename T>
        const T* getInHierarchy(EComponentSearchOrigin searchOrigin) const;

        /**
         * \brief Assigns the given component instance to the linked entity
         * \tparam T The component's type
         * \param instance The added component instance
         * \return A reference to the created or modified component
         */
        template <typename T>
        T& set(const T& instance);

        /**
         * \brief Creates or modifies the linked entity's component of the given type
         * \tparam T The component's type
         * \tparam Args The component's construction parameters
         * \param args The component's construction parameters
         * \return A reference to the created or modified component
         */
        template <typename T, typename... Args>
        T& make(Args&&... args);

        /**
         * \brief Removes the component of the given type from the linked entity
         * \tparam T The component's type
         */
        template <typename T>
        void remove();

        /**
         * \brief Removes the given component instance from the linked entity
         * \tparam T The component's type
         * \param instance The component instance to remove
         */
        template <typename T>
        void remove(const T& instance);

    private:
        Scene* m_scene;
        Entity m_entity;
    };
}

#include "PantheonCore/ECS/EntityHandle.inl"
