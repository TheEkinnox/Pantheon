#pragma once
#include "PantheonCore/Resources/ResourceRef.h"

#include "PantheonScripting/LuaTypeRegistry.h"

namespace PantheonScripting
{
    inline LuaTypeRegistry& LuaTypeRegistry::getInstance()
    {
        static LuaTypeRegistry instance;
        return instance;
    }

    template <typename T>
    LuaTypeInfo& LuaTypeRegistry::registerType(const std::string& name)
    {
        using namespace PantheonCore::ECS;
        using namespace PantheonCore::Resources;
        using namespace PantheonCore::Serialization;

        using EntitiesMap = LuaTypeInfo::EntitiesMap;
        using JsonWriter = LuaTypeInfo::JsonWriter;

        const LuaTypeInfo typeInfo
        {
            .toLua = [](void* object, sol::state& luaState)
            -> sol::userdata
            {
                if (!object)
                    return make_object_userdata(luaState, sol::nil);

                return sol::make_object_userdata(luaState, *static_cast<T*>(object));
            },
            .fromLua = [](void* out, const sol::userdata& object)
            {
                if (!out || !object.is<T>())
                    return false;

                *static_cast<T*>(out) = *object.as<T*>();

                return true;
            },
            .toBinary = [](const sol::userdata& object, std::vector<char>& out, [[maybe_unused]] const EntitiesMap& toSerialized)
            {
                if (!object.valid() || !object.is<T>())
                    return false;

                const T& obj = *object.as<T*>();

                if constexpr (std::is_base_of_v<ResourceRefBase, T> || std::is_base_of_v<IByteSerializable, T>)
                    return obj.toBinary(out);
                else
                    return ComponentRegistry::toBinary<T>(obj, out, toSerialized);
            },
            .fromBinary = [](lua_State* luaState, const char* data, size_t length, size_t& readBytes, [[maybe_unused]] Scene* scene)
            -> sol::optional<sol::object>
            {
                T out;

                if constexpr (std::is_base_of_v<ResourceRefBase, T> || std::is_base_of_v<IByteSerializable, T>)
                    readBytes = out.fromBinary(data, length);
                else
                    readBytes = ComponentRegistry::fromBinary(out, data, length, scene);

                if (readBytes > 0)
                    return sol::make_object(luaState, out);

                return sol::nullopt;
            },
            .toJson = [](const sol::userdata& object, JsonWriter& writer, [[maybe_unused]] const EntitiesMap& toSerialized)
            {
                if (!object.valid() || !object.is<T>())
                    return false;

                const T& obj = *object.as<T*>();

                if constexpr (std::is_base_of_v<ResourceRefBase, T> || std::is_base_of_v<IJsonSerializable, T>)
                    return obj.toJson(writer);
                else
                    return ComponentRegistry::toJson<T>(obj, writer, toSerialized);
            },
            .fromJson =
            [](lua_State* luaState, const rapidjson::Value& json, [[maybe_unused]] Scene* scene)
            -> sol::optional<sol::object>
            {
                T    out;
                bool result;

                if constexpr (std::is_base_of_v<ResourceRefBase, T> || std::is_base_of_v<IJsonSerializable, T>)
                    result = out.fromJson(json);
                else
                    result = ComponentRegistry::fromJson(out, json, scene);

                if (result)
                    return sol::make_object(luaState, out);

                return sol::nullopt;
            }
        };

        return TypeRegistry::registerType<T>(name, typeInfo);
    }
}
