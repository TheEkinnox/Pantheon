#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/EntityHandle.h"

namespace PantheonCore::ECS
{
    struct ComponentHandle
    {
        EntityHandle    m_owner;
        Utility::TypeId m_typeId;

        /**
         * \brief Checks whether the given handle references the same component as this one or not
         * \param other The component handle to compare against
         * \return True if the given handle references the same component. False otherwise
         */
        bool operator==(const ComponentHandle& other) const;

        /**
         * \brief Checks whether the component handle is valid or not
         */
        operator bool() const;

        /**
         * \brief Gets the handled component cast to the given type
         * \tparam T The expected component type
         * \return The found component cast to the given type on success. Nullptr otherwise
         */
        template <typename T = void>
        T* get() const
        {
            if (!m_owner || m_typeId == 0)
                return nullptr;

            return static_cast<T*>(m_owner.getScene()->getStorage(m_typeId).findRaw(m_owner));
        }

        /**
         * \brief Gets or creates the handled component cast to the given type
         * \tparam T The expected component type
         * \return The found or created component cast to the given type on success. Nullptr otherwise
         */
        template <typename T = void>
        T* getOrCreate() const
        {
            if (!m_owner || m_typeId == 0)
                return nullptr;

            return static_cast<T*>(m_owner.getScene()->getStorage(m_typeId).getOrCreateRaw(m_owner));
        }
    };

    template <>
    bool ComponentRegistry::toBinary(const ComponentHandle& value, std::vector<char>& out, const EntitiesMap& toSerialized);

    template <>
    size_t ComponentRegistry::fromBinary(ComponentHandle& out, const char* data, size_t length, Scene* scene);

    template <>
    bool ComponentRegistry::toJson(
        const ComponentHandle& value, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

    template <>
    bool ComponentRegistry::fromJson(ComponentHandle& out, const Serialization::JsonValue& json, Scene* scene);
}
