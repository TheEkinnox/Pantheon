#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/EntityHandle.h"

namespace PantheonScripting
{
    struct LuaComponentHandle
    {
        PantheonCore::ECS::EntityHandle m_owner;
        PantheonCore::Utility::TypeId   m_typeId;

        /**
         * \brief Checks whether the given handle references the same component as this one or not
         * \param other The component handle to compare against
         * \return True if the given handle references the same component. False otherwise
         */
        bool operator==(const LuaComponentHandle& other) const;

        /**
         * \brief Checks whether the component handle is valid or not
         */
        operator bool() const;
    };
}

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(
        const PantheonScripting::LuaComponentHandle& value, std::vector<char>& out, const EntitiesMap& toSerialized);

    template <>
    size_t ComponentRegistry::fromBinary(
        PantheonScripting::LuaComponentHandle& out, const char* data, size_t length, Scene* scene);

    template <>
    bool ComponentRegistry::toJson(
        const PantheonScripting::LuaComponentHandle& value, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

    template <>
    bool ComponentRegistry::fromJson(
        PantheonScripting::LuaComponentHandle& out, const Serialization::JsonValue& json, Scene* scene);
}
