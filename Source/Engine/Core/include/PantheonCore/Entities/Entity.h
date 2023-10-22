#pragma once
#include <memory> // shared_ptr
#include <Transform.h>
#include <vector>

#include "PantheonCore/DataStructure/Node.h"
#include "PantheonCore/Entities/Component.h"

namespace PantheonEngine::Core::Entities
{
    class Entity : public DataStructure::Node, public LibMath::Transform
    {
        using ComponentPtr = std::shared_ptr<Component>;
        using ComponentList = std::vector<ComponentPtr>;

    public:
        Entity() = default;
        Entity(Entity* parent, const Transform& transform);
        Entity(const Entity& other);
        Entity(Entity&& other) noexcept;
        ~Entity() override;

        /**
         * \brief Checks whether the entity is valid or not
         * \return True if the entity is valid. False otherwise.
         */
        explicit operator bool() const;

        Entity& operator=(const Entity& other);
        Entity& operator=(Entity&& other) noexcept;

        /**
         * \brief Adds a component of the given type to the entity
         * \param args The component's constructor's parameters
         * \return A reference to the added component
         */
        template <typename T, typename... Args>
        T& addComponent(Args&&... args);

        template <typename T>
        void removeComponent();

        void removeComponent(const Component& component);

        void removeComponent(Component::ComponentId id);

        template <typename T>
        T* getComponent();

        template <typename T>
        T* getComponent(Component::ComponentId id);

        template <typename T>
        std::vector<std::shared_ptr<T>> getComponents();

        /**
         * \brief Adds the given node as a child of the current node
         * \param child A pointer to the child to add to the current node
         */
        void addChild(Node& child) override;

        /**
         * \brief Removes the given node from this node's children
         * \param child A pointer to the child to remove from the node's children
         */
        void removeChild(Node& child) override;

        /**
         * \brief Updates the entity (does nothing by default)
         */
        virtual void update();

        /**
         * \brief Checks whether the entity is active or not
         * \return True if the entity is currently active. False otherwise.
         */
        bool isActive() const;

        /**
         * \brief Sets whether the entity is active or not
         * \param active The entity's new active state
         */
        void setActive(bool active);

    private:
        ComponentList m_components;
        bool          m_isActive;
        bool          m_isDestroyed;
    };
}

#include "PantheonCore/Entities/Entity.inl"
