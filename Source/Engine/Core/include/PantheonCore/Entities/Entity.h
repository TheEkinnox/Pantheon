#pragma once
#include <memory> // shared_ptr
#include <Transform.h>
#include <vector>

#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/IJsonSerializable.h"
#include "PantheonCore/DataStructure/Node.h"
#include "PantheonCore/Entities/Component.h"

namespace PantheonCore::Entities
{
    class Entity final : public DataStructure::Node, public LibMath::Transform, public Serialization::IByteSerializable,
                         public Serialization::IJsonSerializable
    {
        using ComponentPtr = std::shared_ptr<Component>;
        using ComponentList = std::vector<ComponentPtr>;

    public:
        Entity();
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
        const T* getComponent() const;

        template <typename T>
        T* getComponent(Component::ComponentId id);

        template <typename T>
        const T* getComponent(Component::ComponentId id) const;

        template <typename T>
        std::vector<std::shared_ptr<T>> getComponents();

        template <typename T>
        std::vector<std::shared_ptr<const T>> getComponents() const;

        /**
         * \brief Updates the entity's components and children
         */
        void update();

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

        /**
         * \brief Serializes the entity to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the entity from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool deserialize(const rapidjson::Value& json) override;

        /**
         * \brief Serializes the entity to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the entity from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        bool deserialize(const void* data, size_t length) override;

    protected:
        /**
         * \brief Adds the given node as a child of the current node
         * \param child A pointer to the child to add to the current node
         */
        void onChildAdded(Node& child) override;

        /**
         * \brief Removes the given node from this node's children
         * \param child A pointer to the child to remove from the node's children
         */
        void onRemoveChild(Node& child) override;

    private:
        ComponentList m_components;
        bool          m_isActive;
        bool          m_isDestroyed;

        /**
         * \brief Serializes the entity's transform to json
         * \param writer The output json writer
         * \return True on success. False otherwise
         */
        bool serializeTransform(rapidjson::Writer<rapidjson::StringBuffer>& writer) const;

        /**
         * \brief Deserializes the entity's transform from json
         * \param jsonTransform The transform's json representation
         * \return True on success. False otherwise.
         */
        bool deserializeTransform(const rapidjson::Value& jsonTransform);

        /**
         * \brief Serializes the given component to json
         * \param component The component to serialize
         * \param writer The output json writer
         * \return True on success. False otherwise
         */
        static bool serializeComponent(const ComponentPtr& component, rapidjson::Writer<rapidjson::StringBuffer>& writer);

        /**
         * \brief Deserializes and adds the given json component to the entity
         * \param jsonComponent The added component's json representation
         * \return True on success. False otherwise.
         */
        bool addComponentFromJson(const rapidjson::Value& jsonComponent);

        /**
         * \brief Serializes the entity's transform to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise
         */
        bool serializeTransform(std::vector<char>& output) const;

        /**
         * \brief Deserializes the entity's transform from memory
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True on success. False otherwise.
         */
        bool deserializeTransform(const char* data, size_t length);

        /**
         * \brief Serializes the given component to a byte array
         * \param component The component to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise
         */
        static bool serializeComponent(const ComponentPtr& component, std::vector<char>& output);

        /**
         * \brief Deserializes and adds the given memory component to the entity
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The size of the component block on success. 0 otherwise.
         */
        size_t addComponentFromMemory(const char* data, size_t length);

        /**
         * \brief Deserializes and adds the given memory child entity to the entity
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The size of the child block on success. 0 otherwise.
         */
        size_t addChildFromMemory(const char* data, size_t length);
    };
}

#include "PantheonCore/Entities/Entity.inl"
