#pragma once
#include <cstdint>

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

    private:
        friend class Entity;
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
