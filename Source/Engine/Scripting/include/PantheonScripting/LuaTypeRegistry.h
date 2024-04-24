#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/Utility/TypeRegistry.h"

#include <sol/sol.hpp>

namespace PantheonScripting
{
    struct LuaTypeInfo
    {
        using EntitiesMap = PantheonCore::ECS::ComponentRegistry::EntitiesMap;
        using JsonWriter = PantheonCore::Serialization::JsonWriter;
        using JsonValue = PantheonCore::Serialization::JsonValue;

        /* Lua conversion */
        sol::userdata (*toLua)(void* object, sol::state& luaState);

        bool (*fromLua)(void* out, const sol::userdata& object);

        /* Binary serialization */
        bool (*toBinary)(const sol::userdata& object, std::vector<char>& out, const EntitiesMap& toSerialized);

        sol::optional<sol::object> (*fromBinary)(
            lua_State* luaState, const char* data, size_t length, size_t& readBytes, PantheonCore::ECS::Scene* scene);

        /* Json serialization */
        bool (*toJson)(const sol::userdata& component, JsonWriter& writer, const EntitiesMap& toSerialized);

        sol::optional<sol::object> (*fromJson)(lua_State* luaState, const JsonValue& json, PantheonCore::ECS::Scene* scene);
    };

    class LuaTypeRegistry final : public PantheonCore::Utility::TypeRegistry<LuaTypeInfo>
    {
    public:
        static LuaTypeRegistry& getInstance();

        template <typename T>
        LuaTypeInfo& registerType(const std::string& name);
    };
}

#include "PantheonScripting/LuaTypeRegistry.inl"
