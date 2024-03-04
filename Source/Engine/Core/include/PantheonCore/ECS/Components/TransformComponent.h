#pragma once
#include <Transform.h>

#include "PantheonCore/ECS/Component.h"
#include "PantheonCore/Eventing/Event.h"

namespace PantheonCore::ECS
{
    class TransformComponent : public Component, public LibMath::Transform
    {
        REGISTERED_COMPONENT_BODY(TransformComponent)

    public:
        Eventing::Event<> m_changedEvent;

        /**
         * \brief Creates a default transform component with the given owner
         * \param owner The component's owner
         */
        explicit TransformComponent(Entity& owner);

        /**
         * \brief Creates a transform component with the given owner and transform
         * \param owner The component's owner
         * \param transform The component's transform
         */
        TransformComponent(Entity& owner, const Transform& transform);

        /**
         * \brief Creates a transform component with the given owner and moved transform
         * \param owner The component's owner
         * \param transform The component's transform
         */
        TransformComponent(Entity& owner, Transform&& transform);

        /**
         * \brief Creates a copy of the given transform component
         * \param other The transform component to copy
         */
        TransformComponent(const TransformComponent& other);

        /**
         * \brief Creates a move copy of the given transform component
         * \param other The transform component to move
         */
        TransformComponent(TransformComponent&& other) noexcept;

        /**
         * \brief Destroys the transform component
         */
        ~TransformComponent() override;

        /**
         * \brief Assigns a copy of the given component to this one
         * \param other The transform component to copy
         * \return A reference to the modified component
         */
        TransformComponent& operator=(const TransformComponent& other);

        /**
         * \brief Moves the given transform component into this one
         * \param other The transform component to move
         * \return A reference to the modified component
         */
        TransformComponent& operator=(TransformComponent&& other) noexcept;

        /**
         * \brief Serializes the transform to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the transform from memory
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

        /**
         * \brief Serializes the transform to json
         * \param writer The output json writer
         * \return True on success. False otherwise
         */
        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the transform from json
         * \param json The transform's json representation
         * \return True on success. False otherwise.
         */
        bool deserialize(const rapidjson::Value& json) override;

    private:
        Eventing::Event<>::ListenerId m_childAddedListenerId   = 0;
        Eventing::Event<>::ListenerId m_childRemovedListenerId = 0;

        using Transform::setParent;

        /**
         * \brief Binds the necessary actions to the transform's corresponding events
         */
        void bindEvents();

        /**
         * \brief Unbinds the necessary actions from the transform's corresponding events
         */
        void unbindEvents();

        /**
         * \brief The action to perform when the component is added
         */
        bool onAdd() override;

        /**
         * \brief The action to perform when the transform changed
         */
        void onChange() override;

        /**
         * \brief The action to perform when the component is removed
         */
        void onRemove() override;
    };
}
