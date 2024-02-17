#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/IJsonSerializable.h"

#include <cstdint>
#include <string>
#include <unordered_map>

#define REGISTER_COMPONENT_TYPE(Name, Type)                                                       \
static uint8_t compReg_##Name = (PantheonCore::ECS::Component::registerType<Type>(#Name), 0);

#define REGISTERED_COMPONENT_BODY(Type)                                 \
public:                                                                 \
inline const std::string& Type::getTypeName() const override            \
{                                                                       \
    return PantheonCore::ECS::Component::getRegisteredTypeName<Type>(); \
}                                                                       \
private:


namespace PantheonCore::ECS
{
    class Entity;
    class Component;

    template <typename T, typename... Args>
    T* createComponent(Entity& owner, Args&&... args);

    class Component : public Serialization::IByteSerializable, public Serialization::IJsonSerializable
    {
    public:
        using ComponentId = uint64_t;

        Component(const Component& other);
        Component(Component&& other) noexcept;

        Component& operator=(const Component& other);
        Component& operator=(Component&& other) noexcept;

        bool operator==(const Component& other) const;
        bool operator!=(const Component& other) const;

        ~Component() override;

        /**
         * \brief Registers the given component type (required for the create function)
         * \tparam T The component type to register
         */
        template <typename T>
        static void registerType(const std::string& name);

        /**
         * \brief Gets the registered name for the given component type
         * \tparam T The component type
         * \return The registered name for the given component type
         */
        template <typename T>
        static const std::string& getRegisteredTypeName();

        /**
         * \brief Tries to allocate a component of the given registered component type.
         * \param type The type of the component to create
         * \param owner The created component's owner
         * \return A pointer to the allocated resource on success, nullptr otherwise
         */
        inline static Component* create(const std::string& type, Entity& owner);

        /**
         * \brief Gets the component's registered type name
         * \return The component's registered type name
         */
        virtual const std::string& getTypeName() const = 0;

        /**
         * \brief Updates the component
         */
        virtual void update()
        {
        }

        /**
         * \brief Checks whether the component is active or not
         * \return True if the component is currently active. False otherwise.
         */
        bool isActive() const;

        /**
         * \brief Sets whether the component is active or not
         * \param active The component's new active state
         */
        void setActive(bool active);

        /**
         * \brief Gets the component's id
         * \return The component's id
         */
        ComponentId getId() const;

        /**
         * \brief Gets the component's owner
         * \return A reference to the component's owner
         */
        Entity& getOwner() const;

    protected:
        explicit Component(Entity& owner);

        /**
         * \brief The action to perform when the component is added
         */
        virtual void onAdd()
        {
        }

        /**
         * \brief The action to perform when the component is removed
         */
        virtual void onRemove()
        {
        }

        /**
         * \brief The action to perform when the components gets enabled
         */
        virtual void onEnable()
        {
        }

        /**
         * \brief The action to perform when the components gets disabled
         */
        virtual void onDisable()
        {
        }

    private:
        friend class Entity;

        using AllocFunc = decltype(&createComponent<Component>);
        using TypeMap = std::unordered_map<std::string, AllocFunc>;
        using TypeNameMap = std::unordered_map<size_t, std::string>;

        inline static TypeMap     s_componentTypes{};
        inline static TypeNameMap s_componentTypeNames{};
        inline static ComponentId s_currentId = 1;

        Entity*     m_owner;
        ComponentId m_id;
        bool        m_isActive = true;
    };
}

#include "PantheonCore/ECS/Component.inl"
