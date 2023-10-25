#pragma once
#include <cstdint>
#include <memory> // shared_ptr
#include <string>
#include <unordered_map>

#define REGISTER_COMPONENT_TYPE(Name, Type)                                                            \
static uint8_t reg_##Name = (PantheonEngine::Core::Entities::Component::registerType<Type>(#Name), 0);

#define REGISTERED_COMPONENT_BODY(Type)                                              \
public:                                                                              \
inline std::string Type::getTypeName() const override                                \
{                                                                                    \
    return PantheonEngine::Core::Entities::Component::getRegisteredTypeName<Type>(); \
}

namespace PantheonEngine::Core::Entities
{
    class Entity;

    class Component
    {
    public:
        using ComponentId = uint64_t;

        Component(const Component& other);
        Component(Component&& other) noexcept;

        Component& operator=(const Component& other);
        Component& operator=(Component&& other) noexcept;

        bool operator==(const Component&) const;
        bool operator!=(const Component&) const;

        virtual ~Component();

        /**
         * \brief Registers the given component type (required for the create function)
         * \tparam T The component type to register
         */
        template <typename T>
        static constexpr void registerType(const std::string& name);

        /**
         * \brief Tries to allocate a component of the given registered component type.
         * \param type The type of the component to create
         * \param owner The created component's owner
         * \return A pointer to the allocated resource on success, nullptr otherwise
         */
        inline static std::shared_ptr<Component> create(const std::string& type, Entity& owner);

        /**
         * \brief Gets the component's registered type name
         * \return The component's registered type name
         */
        virtual std::string getTypeName() const = 0;

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
         * \brief Gets the registered name for the given component type
         * \tparam T The component type
         * \return The registered name for the given component type
         */
        template <typename T>
        static std::string getRegisteredTypeName();

    private:
        friend class Entity;
        using AllocFunc = std::shared_ptr<Component> (*)(Entity& owner);
        using TypeMap = std::unordered_map<std::string, AllocFunc>;
        using TypeNameMap = std::unordered_map<size_t, std::string>;

        inline static TypeMap     s_componentTypes{};
        inline static TypeNameMap s_componentTypeNames{};
        inline static ComponentId s_currentId = 1;

        Entity*     m_owner;
        ComponentId m_id;
        bool        m_isActive = true;

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
    };
}

#include "PantheonCore/Entities/Component.inl"
