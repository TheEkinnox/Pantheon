#include "PantheonScripting/LuaScriptComponent.h"

#include "PantheonScripting/LuaContext.h"

namespace PantheonScripting
{
    using namespace PantheonCore::ECS;

    std::optional<sol::object> luaObjectFromJson(lua_State* luaState, const rapidjson::Value& json)
    {
        if (!CHECK(luaState, "Unable to deserialize lua object - No lua state"))
            return std::nullopt;

        auto it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd() && it->value.IsInt(), "Unable to deserialize lua object - Type should be an int"))
            return std::nullopt;

        const sol::type objectType = static_cast<sol::type>(it->value.GetInt());

        it = json.FindMember("data");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua object - Missing value"))
            return std::nullopt;

        switch (objectType)
        {
        case sol::type::lua_nil:
        {
            if (!CHECK(it->value.IsNull(), "Unable to deserialize lua object - Expected null value"))
                return std::nullopt;

            return sol::nil;
        }
        case sol::type::string:
        {
            if (!CHECK(it->value.IsString(), "Unable to deserialize lua string"))
                return std::nullopt;

            return sol::make_object(luaState, std::string(it->value.GetString(), it->value.GetStringLength()));
        }
        case sol::type::number:
        {
            if (!CHECK(it->value.IsNumber(), "Unable to deserialize lua number"))
                return std::nullopt;

            return sol::make_object(luaState, it->value.GetDouble());
        }
        case sol::type::boolean:
        {
            if (!CHECK(it->value.IsBool(), "Unable to deserialize lua bool"))
                return std::nullopt;

            return sol::make_object(luaState, it->value.GetBool());
        }
        case sol::type::table:
        {
            sol::table table(luaState, sol::create);

            if (!ComponentRegistry::fromJson(table, it->value))
                return std::nullopt;

            return table;
        }
        case sol::type::userdata:
        {
            sol::userdata userdata(luaState);
            // TODO: lua user data from json
            return std::nullopt;
        }
        case sol::type::lightuserdata:
        {
            // TODO: lua light user data from json
            return std::nullopt;
        }
        case sol::type::none:
        case sol::type::thread:
        case sol::type::function:
        case sol::type::poly:
        default:
            CHECK(false, "Unable to deserialize lua object - Unsupported type");
            return std::nullopt;
        }
    }
}

namespace PantheonCore::ECS
{
    using namespace PantheonScripting;

    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, LuaScriptComponent& component)
    {
        LuaContext& context = PTH_SERVICE(LuaContext);
        (void)context.addScript(entity, component);
    }

    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, LuaScriptComponent&)
    {
        LuaContext& context = PTH_SERVICE(LuaContext);
        context.removeScript(entity);
    }

    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, LuaScriptComponent& component)
    {
        onRemove(entity, component);
    }

    template <>
    void ComponentTraits::onChange(EntityHandle& entity, LuaScriptComponent& component)
    {
        onAdd(entity, component);
    }

    template <>
    bool ComponentRegistry::toJson(
        const LuaScriptComponent& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        writer.StartObject();

        writer.Key("script");
        if (!component.m_script.toJson(writer))
            return false;

        writer.Key("data");
        if (!toJson(component.m_table, writer, toSerialized))
            return false;

        return CHECK(writer.EndObject(), "Failed to serialize lua script component");
    }

    template <>
    bool ComponentRegistry::fromJson(LuaScriptComponent& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to serialize lua script component - Json value should be an object"))
            return false;

        auto it = json.FindMember("script");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua script component - Missing script ref")
            || !out.m_script.fromJson(it->value))
            return false;

        it = json.FindMember("data");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua script component - Missing script data"))
            return false;

        out.m_table = sol::table(PTH_SERVICE(LuaContext).getLuaState(), sol::create);
        return fromJson(out.m_table, it->value);
    }

    template <>
    bool ComponentRegistry::toJson(
        const sol::table& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        if (component == sol::nil)
            return writer.Null();

        writer.StartArray();

        for (auto [key, value] : component)
        {
            const sol::type valType = value.get_type();

            if (valType == sol::type::function || valType == sol::type::thread)
                continue;

            writer.StartObject();

            writer.Key("key");
            if (!CHECK(key != sol::nil, "Unable to serialize lua table - Nil key") || !toJson(key, writer, toSerialized))
                return false;

            writer.Key("value");
            if (!toJson(value, writer, toSerialized) || !CHECK(writer.EndObject(), "Unable to write lua table element"))
                return false;
        }

        return CHECK(writer.EndArray(), "Failed to serialize lua table");
    }

    template <>
    bool ComponentRegistry::fromJson(sol::table& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsArray(), "Unable to deserialize lua table - Json value should be an array"))
            return false;

        for (const auto& elem : json.GetArray())
        {
            if (!CHECK(elem.IsObject(), "Unable to deserialize lua table element - Json value should be an object"))
                return false;

            auto it = elem.FindMember("key");

            if (!CHECK(it != elem.MemberEnd(), "Unable to deserialize lua table element - Missing key"))
                return false;

            std::optional key = luaObjectFromJson(out.lua_state(), it->value);

            if (!key.has_value() || !CHECK(*key != sol::nil, "Unable to deserialize lua table - Nil key"))
                return false;

            it = elem.FindMember("value");

            if (!CHECK(it != elem.MemberEnd(), "Unable to deserialize lua table element - Missing value"))
                return false;

            std::optional val = luaObjectFromJson(out.lua_state(), it->value);

            if (!val.has_value())
                return false;

            out[*key] = *val;
        }

        return true;
    }

    template <>
    bool ComponentRegistry::toJson(
        const sol::object& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        if (!CHECK(component.valid(), "Unable to seriliaze lua object - Invalid value"))
            return false;

        const sol::type objectType = component.get_type();

        writer.StartObject();
        writer.Key("type");
        writer.Int(static_cast<int>(objectType));

        writer.Key("data");
        bool result;

        switch (objectType)
        {
        case sol::type::lua_nil:
            result = CHECK(writer.Null(), "Failed to write lua nil object");
            break;
        case sol::type::string:
        {
            const std::string str = component.as<std::string>();
            result = CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Unable to write lua string");
            break;
        }
        case sol::type::number:
            result = CHECK(writer.Double(component.as<double>()), "Unable to write lua number");
            break;
        case sol::type::boolean:
            result = CHECK(writer.Bool(component.as<bool>()), "Unable to write lua bool");
            break;
        case sol::type::table:
            result = toJson(component.as<sol::table>(), writer, toSerialized);
            break;
        case sol::type::userdata:
        {
            // TODO: lua user data to json
            return false;
        }
        case sol::type::lightuserdata:
        {
            // TODO: lua light user data to json
            return false;
        }
        case sol::type::none:
        case sol::type::thread:
        case sol::type::function:
        case sol::type::poly:
        default:
            return CHECK(false, "Unable to serialize lua object - Unsupported type");
        }

        return result && CHECK(writer.EndObject(), "Failed to write lua object");
    }
}
