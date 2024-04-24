#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentTraits.h"
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/Serialization/MathSerializers.h"

namespace PantheonCore::ECS
{
    class HierarchyComponent
    {
    public:
        /**
         * \brief Creates a hierarchy component with the given parent entity
         * \param parent The entity's parent entity
         */
        explicit HierarchyComponent(Entity parent = NULL_ENTITY);

        /**
         * \brief Creates a copy of the given hierarchy component
         * \param other The hierarchy component to copy
         */
        HierarchyComponent(const HierarchyComponent& other) = default;

        /**
         * \brief Creates a move copy of the given hierarchy component
         * \param other The hierarchy component to move
         */
        HierarchyComponent(HierarchyComponent&& other) noexcept = default;

        /**
         * \brief Destroys the hierarchy component
         */
        ~HierarchyComponent() = default;

        /**
         * \brief Assigns a copy of the given hierarchy component to this one
         * \param other The hierarchy component to copy
         * \return A reference to the modified component
         */
        HierarchyComponent& operator=(const HierarchyComponent& other) = default;

        /**
         * \brief Moves the given hierarchy component into this one
         * \param other The hierarchy component to move
         * \return A reference to the modified component
         */
        HierarchyComponent& operator=(HierarchyComponent&& other) noexcept = default;

        /**
         * \brief Gets the hierarchy owner's parent entity
         * \return The owner's parent
         */
        Entity getParent() const;

        /**
         * \brief Sets the hierarchy owner's parent entity
         * \param parent The owner's new parent
         */
        void setParent(Entity parent);

        /**
         * \brief Gets the hierarchy owner's first child
         * \return The owner's first child
         */
        Entity getFirstChild() const;

        /**
         * \brief Gets the hierarchy owner's previous sibling
         * \return The owner's previous sibling
         */
        Entity getPreviousSibling() const;

        /**
         * \brief Gets the hierarchy owner's next sibling
         * \return The owner's next sibling
         */
        Entity getNextSibling() const;

        /**
         * \brief Gets the hierarchy owner's number of child
         * \return The owner's child count
         */
        size_t getChildCount() const;

    private:
        friend struct ComponentTraits;
        friend class ComponentRegistry;

        Entity m_parent          = NULL_ENTITY;
        Entity m_firstChild      = NULL_ENTITY;
        Entity m_previousSibling = NULL_ENTITY;
        Entity m_nextSibling     = NULL_ENTITY;
        size_t m_childCount      = 0;
    };

    std::vector<LibMath::Transform*> getChildTransforms(const EntityHandle& entity);

    /**
     * \brief Links the relevant transforms to this entity
     * \param entity The entity for which the transforms should be linked
     */
    void linkTransforms(EntityHandle& entity);

    /**
     * \brief Unlinks the relevant transforms from this entity
     * \param entity The entity for which the transforms should be unlinked
     */
    void unlinkTransforms(const EntityHandle& entity);

    template <>
    void ComponentTraits::onAdd(EntityHandle&, HierarchyComponent&);

    template <>
    void ComponentTraits::onRemove(EntityHandle&, HierarchyComponent&);

    template <>
    void ComponentTraits::onBeforeChange(EntityHandle&, HierarchyComponent&);

    template <>
    void ComponentTraits::onChange(EntityHandle&, HierarchyComponent&);

    template <>
    void ComponentTraits::onAdd(EntityHandle&, LibMath::Transform&);

    template <>
    void ComponentTraits::onRemove(EntityHandle&, LibMath::Transform&);

    template <>
    void ComponentTraits::onChange(EntityHandle&, LibMath::Transform&);

    template <>
    bool ComponentRegistry::toBinary(const HierarchyComponent&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(HierarchyComponent&, const char*, size_t, Scene*);

    template <>
    bool ComponentRegistry::toJson(const HierarchyComponent&, Serialization::JsonWriter&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(HierarchyComponent&, const Serialization::JsonValue&, Scene*);
}
