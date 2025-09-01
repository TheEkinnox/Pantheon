#pragma once
#include "PantheonScripting/LuaScript.h"

#include <PantheonCore/ECS/EntityHandle.h>
#include <PantheonCore/Resources/ResourceRef.h>

#include <sol/table.hpp>

namespace PantheonScripting
{
    struct LuaScriptHandle
    {
        PantheonCore::Resources::ResourceRef<LuaScript> m_script;
        PantheonCore::ECS::EntityHandle                 m_owner;
        sol::table                                      m_table = sol::lua_nil;

        /**
         * \brief Checks whether this script handle should be ordered before the given one or not
         * \param other The script handle to compare against
         * \return True if this script handle should be ordered before the given one. False otherwise
         */
        bool operator<(const LuaScriptHandle& other) const;

        /**
         * \brief Checks whether the given script handle references the same script as this one or not
         * \param other The script handle to compare against
         * \return True if the given script handle references the same script as this one. False otherwise
         */
        bool operator==(const LuaScriptHandle& other) const;

        /**
         * \brief Checks whether the script handle is valid or not
         */
        operator bool() const;
    };
}

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(
        const PantheonScripting::LuaScriptHandle& value, std::vector<char>& out, const EntitiesMap& toSerialized);

    template <>
    size_t ComponentRegistry::fromBinary(PantheonScripting::LuaScriptHandle& out, const char* data, size_t length, Scene* scene);

    template <>
    bool ComponentRegistry::toJson(
        const PantheonScripting::LuaScriptHandle& value, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

    template <>
    bool ComponentRegistry::fromJson(PantheonScripting::LuaScriptHandle& out, const Serialization::JsonValue& json, Scene* scene);
}
