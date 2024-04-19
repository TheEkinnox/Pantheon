#include "PantheonScripting/LuaScriptList.h"

#include "PantheonScripting/LuaContext.h"
#include "PantheonScripting/LuaTypeRegistry.h"

using namespace PantheonCore::Serialization;

namespace PantheonScripting
{
    using namespace PantheonCore::ECS;

    LuaScriptList::~LuaScriptList()
    {
        clear();
    }

    LuaScriptList& LuaScriptList::operator=(const LuaScriptList& other)
    {
        if (this == &other)
            return *this;

        clear();

        m_owner   = other.m_owner;
        m_scripts = other.m_scripts;

        return *this;
    }

    LuaScriptList& LuaScriptList::operator=(LuaScriptList&& other) noexcept
    {
        if (this == &other)
            return *this;

        clear();

        m_owner   = other.m_owner;
        m_scripts = std::move(other.m_scripts);

        return *this;
    }

    bool LuaScriptList::contains(const std::string& script) const
    {
        return m_scripts.contains(LuaContext::getModuleName(script));
    }

    LuaContext::ScriptHandle LuaScriptList::get(const std::string& script) const
    {
        return PTH_SERVICE(LuaContext).getScript(script, m_owner);
    }

    LuaContext::ScriptHandle LuaScriptList::add(std::string script, const sol::table& hint)
    {
        script = LuaContext::getModuleName(script);

        if (script.empty())
            return {};

        if (!CHECK(!m_scripts.contains(script), "Attempted to add script \"%s\" to entity \"%llu:%u\" more than once",
                script.c_str(), m_owner.getEntity().getIndex(), m_owner.getEntity().getVersion()))
            return {};

        LuaContext::ScriptHandle handle = PTH_SERVICE(LuaContext).addScript(script, m_owner, hint);

        if (!handle.m_table.valid())
            return {};

        // Note: Re-fetch is necessary to avoid issues when entities are created during script initialization
        handle.m_owner.get<LuaScriptList>()->m_scripts[script] = handle.m_table;
        return handle;
    }

    void LuaScriptList::remove(const std::string& script)
    {
        PTH_SERVICE(LuaContext).removeScript(script, m_owner);
        m_scripts.erase(LuaContext::getModuleName(script));
    }

    void LuaScriptList::clear()
    {
        LuaContext& context = PTH_SERVICE(LuaContext);

        for (const auto& script : m_scripts | std::views::keys)
            context.removeScript(script, m_owner);

        m_scripts.clear();
    }

    sol::optional<sol::object> luaObjectFromJson(lua_State* luaState, const rapidjson::Value& json, Scene* scene)
    {
        if (!CHECK(luaState, "Unable to deserialize lua object - No lua state"))
            return sol::nullopt;

        auto it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd() && it->value.IsInt(), "Unable to deserialize lua object - Type should be an int"))
            return sol::nullopt;

        const sol::type objectType = static_cast<sol::type>(it->value.GetInt());

        it = json.FindMember("data");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua object - Missing value"))
            return sol::nullopt;

        switch (objectType)
        {
        case sol::type::lua_nil:
        {
            if (!CHECK(it->value.IsNull(), "Unable to deserialize lua object - Expected null value"))
                return sol::nullopt;

            return sol::make_object(luaState, sol::nil);
        }
        case sol::type::string:
        {
            if (!CHECK(it->value.IsString(), "Unable to deserialize lua string"))
                return sol::nullopt;

            return sol::make_object(luaState, std::string(it->value.GetString(), it->value.GetStringLength()));
        }
        case sol::type::number:
        {
            if (!CHECK(it->value.IsNumber(), "Unable to deserialize lua number"))
                return sol::nullopt;

            return sol::make_object(luaState, it->value.GetDouble());
        }
        case sol::type::boolean:
        {
            if (!CHECK(it->value.IsBool(), "Unable to deserialize lua bool"))
                return sol::nullopt;

            return sol::make_object(luaState, it->value.GetBool());
        }
        case sol::type::table:
        {
            sol::table table(luaState, sol::create);

            if (!ComponentRegistry::fromJson(table, it->value, scene))
                return sol::nullopt;

            return table;
        }
        case sol::type::userdata:
        case sol::type::lightuserdata:
        {
            if (!CHECK(it->value.IsObject(), "Unable to deserialize lua user type - Json value should be an object"))
                return sol::nullopt;

            const auto& jsonData = it->value;

            auto dataIt = jsonData.FindMember("type");

            if (!CHECK(dataIt != jsonData.MemberEnd() && dataIt->value.IsString(),
                    "Unable to deserialize lua user type - Invalid type string"))
                return sol::nullopt;

            const std::string typeString(dataIt->value.GetString(), dataIt->value.GetStringLength());

            const auto& luaTypes = LuaTypeRegistry::getInstance();

            if (!CHECK(luaTypes.contains(typeString), "Unable to deserialize unregistered lua user type %s", typeString.c_str()))
                return sol::nullopt;

            dataIt = jsonData.FindMember("data");

            if (!CHECK(dataIt != jsonData.MemberEnd(), "Unable to deserialize lua user type - Missing data"))
                return sol::nullopt;

            return luaTypes.getTypeInfo(typeString).fromJson(luaState, dataIt->value, scene);
        }
        case sol::type::none:
        case sol::type::thread:
        case sol::type::function:
        case sol::type::poly:
        default:
            CHECK(false, "Unable to deserialize lua object - Unsupported type");
            return sol::nullopt;
        }
    }

    sol::optional<sol::object> luaObjectFromBinary(
        lua_State* luaState, const char* data, size_t length, size_t& readBytes, Scene* scene)
    {
        readBytes = 0;

        if (!CHECK(luaState, "Unable to deserialize lua object - No lua state"))
            return sol::nullopt;

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize lua object - Empty buffer"))
            return sol::nullopt;

        sol::type objectType;
        readBytes = IByteSerializable::readNumber(objectType, data, length);

        if (!CHECK(readBytes > 0, "Failed to read lua object type"))
            return sol::nullopt;

        if (!CHECK(length >= readBytes, "Unable to deserialize lua object - Invalid offset"))
            return sol::nullopt;

        switch (objectType)
        {
        case sol::type::lua_nil:
        {
            return sol::make_object(luaState, sol::nil);
        }
        case sol::type::string:
        {
            std::string out;

            const size_t offset = IByteSerializable::deserializeString(out, data + readBytes, length - readBytes);

            if (!CHECK(offset > 0, "Failed to deserialize lua string"))
                return sol::nullopt;

            readBytes += offset;
            return sol::make_object(luaState, out);
        }
        case sol::type::number:
        {
            double out;

            const size_t offset = IByteSerializable::readNumber(out, data + readBytes, length - readBytes);

            if (!CHECK(offset > 0, "Failed to deserialize lua number"))
                return sol::nullopt;

            readBytes += offset;
            return sol::make_object(luaState, out);
        }
        case sol::type::boolean:
        {
            bool out;

            const size_t offset = IByteSerializable::readNumber(out, data + readBytes, length - readBytes);

            if (!CHECK(offset > 0, "Failed to deserialize lua bool"))
                return sol::nullopt;

            readBytes += offset;
            return sol::make_object(luaState, out);
        }
        case sol::type::table:
        {
            sol::table table(luaState, sol::create);

            const size_t offset = ComponentRegistry::fromBinary(table, data + readBytes, length - readBytes, scene);

            if (offset == 0)
                return sol::nullopt;

            readBytes += offset;
            return table;
        }
        case sol::type::userdata:
        case sol::type::lightuserdata:
        {
            std::string typeString;
            size_t      offset = IByteSerializable::deserializeString(typeString, data + readBytes, length - readBytes);

            if (!CHECK(offset > 0, "Unable to deserialize lua user type - Failed to read type string"))
                return sol::nullopt;

            readBytes += offset;

            const auto& luaTypes = LuaTypeRegistry::getInstance();

            if (!CHECK(luaTypes.contains(typeString), "Unable to deserialize unregistered lua user type %s", typeString.c_str()))
                return sol::nullopt;

            if (!CHECK(length >= readBytes, "Unable to deserialize lua user type data - Invalid offset"))
                return sol::nullopt;

            sol::optional<sol::object> out = luaTypes.getTypeInfo(typeString).fromBinary(
                luaState, data + readBytes, length - readBytes, offset, scene);

            if (!out.has_value() || offset == 0)
                return sol::nullopt;

            readBytes += offset;
            return out;
        }
        case sol::type::none:
        case sol::type::thread:
        case sol::type::function:
        case sol::type::poly:
        default:
            CHECK(false, "Unable to deserialize lua object - Unsupported type");
            return sol::nullopt;
        }
    }
}

namespace PantheonCore::ECS
{
    using namespace PantheonScripting;

    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, LuaScriptList& component)
    {
        component.m_owner = entity;

        LuaContext& context = PTH_SERVICE(LuaContext);

        for (const auto& [script, hint] : component.m_scripts)
            context.addScript(script, entity, hint);
    }

    template <>
    void ComponentTraits::onRemove(EntityHandle&, LuaScriptList& component)
    {
        component.clear();
    }

    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, LuaScriptList& component)
    {
        onRemove(entity, component);
    }

    template <>
    void ComponentTraits::onChange(EntityHandle& entity, LuaScriptList& component)
    {
        onAdd(entity, component);
    }

    template <>
    bool ComponentRegistry::toJson(
        const LuaScriptList& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        writer.StartArray();

        for (const auto& script : component.m_scripts | std::views::keys)
        {
            writer.StartObject();

            writer.Key("name");

            if (!CHECK(writer.String(script.c_str(), static_cast<rapidjson::SizeType>(script.size())),
                    "Failed to write lua script name"))
                return false;

            writer.Key("data");

            if (!toJson(component.get(script).m_table, writer, toSerialized))
                return false;

            if (!CHECK(writer.EndObject(), "Failed to serialize lua script"))
                return false;
        }

        return CHECK(writer.EndArray(), "Failed to serialize lua script list");
    }

    template <>
    bool ComponentRegistry::fromJson(LuaScriptList& out, const rapidjson::Value& json, Scene* scene)
    {
        out.clear();

        if (!CHECK(json.IsArray(), "Unable to deserialize lua script list - Invalid array"))
            return false;

        for (const auto& script : json.GetArray())
        {
            auto scriptIt = script.FindMember("name");

            if (!CHECK(scriptIt != script.MemberEnd() && scriptIt->value.IsString(),
                    "Unable to deserialize lua script - Invalid script name"))
                return false;

            std::string name(scriptIt->value.GetString(), scriptIt->value.GetStringLength());
            name = LuaContext::getModuleName(name);

            if (out.m_scripts.contains(name))
            {
                DEBUG_LOG("[WARNING] Ignoring duplicate lua script \"%s\"", name.c_str());
                continue;
            }

            scriptIt = script.FindMember("data");

            if (!CHECK(scriptIt != script.MemberEnd(), "Unable to deserialize lua script - Missing script data"))
                return false;

            sol::table table(PTH_SERVICE(LuaContext).getLuaState(), sol::create);

            if (!fromJson(table, scriptIt->value, scene))
                return false;

            out.m_scripts[name] = table;
        }

        return true;
    }

    template <>
    bool ComponentRegistry::toBinary(const LuaScriptList& component, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        const auto scriptCount = static_cast<IByteSerializable::ElemCountT>(component.m_scripts.size());

        if (!CHECK(IByteSerializable::writeNumber(scriptCount, out), "Failed to write lua script count"))
            return false;

        for (const auto& script : component.m_scripts | std::views::keys)
        {
            if (!CHECK(IByteSerializable::serializeString(script, out), "Failed to write script name")
                || !toBinary(component.get(script).m_table, out, toSerialized))
                return false;
        }

        return true;
    }

    template <>
    size_t ComponentRegistry::fromBinary(LuaScriptList& out, const char* data, size_t length, Scene* scene)
    {
        out.clear();

        IByteSerializable::ElemCountT scriptCount = 0;

        size_t offset = IByteSerializable::readNumber(scriptCount, data, length);

        if (!CHECK(offset > 0, "Unable to deserialize lua script list - Failed to read script count"))
            return 0;

        for (; scriptCount > 0; --scriptCount)
        {
            if (!CHECK(length >= offset, "Unable to deserialize lua script name - Invalid offset"))
                return 0;

            std::string script;
            size_t      readBytes = IByteSerializable::deserializeString(script, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Unable to deserialize lua script - Failed to read script name"))
                return 0;

            offset += readBytes;

            script = LuaContext::getModuleName(script);

            if (out.m_scripts.contains(script))
            {
                DEBUG_LOG("[WARNING] Ignoring duplicate lua script \"%s\"", script.c_str());
                continue;
            }

            if (!CHECK(length >= offset, "Unable to deserialize lua script - Invalid offset"))
                return 0;

            sol::table table(PTH_SERVICE(LuaContext).getLuaState(), sol::create);
            readBytes = fromBinary(table, data + offset, length - offset, scene);

            if (readBytes == 0)
                return false;

            out.m_scripts[script] = table;
            offset += readBytes;
        }

        return offset;
    }

    template <>
    bool ComponentRegistry::toJson(
        const sol::table& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        if (component == sol::nil)
            return writer.Null();

        writer.StartArray();

        for (const auto& [key, value] : component)
        {
            const sol::type valType = value.get_type();

            if (valType == sol::type::function || valType == sol::type::thread)
                continue;

            writer.StartObject();

            writer.Key("key");
            if (!CHECK(key != sol::nil, "Unable to serialize lua table - Nil key") || !toJson(key, writer, toSerialized))
                return false;

            writer.Key("value");
            if (!toJson(value, writer, toSerialized) || !CHECK(writer.EndObject(), "Unable to write lua table member"))
                return false;
        }

        return CHECK(writer.EndArray(), "Failed to serialize lua table");
    }

    template <>
    bool ComponentRegistry::fromJson(sol::table& out, const rapidjson::Value& json, Scene* scene)
    {
        if (!CHECK(json.IsArray(), "Unable to deserialize lua table - Json value should be an array"))
            return false;

        for (const auto& elem : json.GetArray())
        {
            if (!CHECK(elem.IsObject(), "Unable to deserialize lua table member - Json value should be an object"))
                return false;

            auto it = elem.FindMember("key");

            if (!CHECK(it != elem.MemberEnd(), "Unable to deserialize lua table member - Missing key"))
                return false;

            sol::optional key = luaObjectFromJson(out.lua_state(), it->value, scene);

            if (!key.has_value() || !CHECK(*key != sol::nil, "Unable to deserialize lua table member - Nil key"))
                return false;

            it = elem.FindMember("value");

            if (!CHECK(it != elem.MemberEnd(), "Unable to deserialize lua table member - Missing value"))
                return false;

            sol::optional val = luaObjectFromJson(out.lua_state(), it->value, scene);

            if (!val.has_value())
                return false;

            out[*key] = *val;
        }

        return true;
    }

    template <>
    bool ComponentRegistry::toBinary(const sol::table& component, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        IByteSerializable::ElemCountT count = 0;

        for (auto it = component.cbegin(); it != component.cend(); ++it)
        {
            const sol::type valType = (*it).second.get_type();

            if (valType != sol::type::function && valType != sol::type::thread)
                ++count;
        }

        if (!CHECK(IByteSerializable::writeNumber(count, out), "Unable to serialize lua table - Failed to write member count"))
            return false;

        for (const auto& [key, value] : component)
        {
            const sol::type valType = value.get_type();

            if (valType == sol::type::function || valType == sol::type::thread)
                continue;

            if (!CHECK(key != sol::nil, "Unable to serialize lua table member - Nil key") || !toBinary(key, out, toSerialized))
                return false;

            if (!toBinary(value, out, toSerialized))
                return false;
        }

        return true;
    }

    template <>
    size_t ComponentRegistry::fromBinary(sol::table& out, const char* data, size_t length, Scene* scene)
    {
        IByteSerializable::ElemCountT count;

        size_t offset = IByteSerializable::readNumber(count, data, length);

        if (!CHECK(offset > 0, "Failed to read lua table member count"))
            return 0;

        for (IByteSerializable::ElemCountT i = 0; i < count; ++i)
        {
            if (!CHECK(length >= offset, "Unable to deserialize lua table member key - Invalid offset"))
                return 0;

            size_t readBytes = 0;

            sol::optional key = luaObjectFromBinary(out.lua_state(), data + offset, length - offset, readBytes, scene);

            if (!key.has_value() || readBytes == 0 || !CHECK(*key != sol::nil, "Unable to deserialize lua table member - Nil key"))
                return 0;

            offset += readBytes;

            if (!CHECK(length >= offset, "Unable to deserialize lua table member value - Invalid offset"))
                return 0;

            sol::optional val = luaObjectFromBinary(out.lua_state(), data + offset, length - offset, readBytes, scene);

            if (!val.has_value() || readBytes == 0)
                return 0;

            offset += readBytes;

            out[*key] = *val;
        }

        return offset;
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
        case sol::type::lightuserdata:
        {
            const auto&       userData   = component.as<sol::userdata&>();
            const std::string typeString = userData["__type"]["name"];

            const auto& luaTypes = LuaTypeRegistry::getInstance();

            if (!CHECK(luaTypes.contains(typeString), "Unable to serialize unregistered lua user type %s", typeString.c_str()))
                return false;

            writer.StartObject();

            writer.Key("type");
            writer.String(typeString.c_str(), static_cast<rapidjson::SizeType>(typeString.size()));

            writer.Key("data");

            if (!luaTypes.getTypeInfo(typeString).toJson(userData, writer, toSerialized))
                return false;

            result = CHECK(writer.EndObject(), "Failed to write lua user data");
            break;
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

    template <>
    bool ComponentRegistry::toBinary(const sol::object& component, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        if (!CHECK(component.valid(), "Unable to seriliaze lua object - Invalid value"))
            return false;

        const sol::type objectType = component.get_type();

        if (!CHECK(IByteSerializable::writeNumber(static_cast<int>(objectType), out), "Failed to write lua object type"))
            return false;

        switch (objectType)
        {
        case sol::type::lua_nil:
            return true;
        case sol::type::string:
        {
            return CHECK(IByteSerializable::serializeString(component.as<std::string>(), out), "Failed to write lua string");
        }
        case sol::type::number:
        {
            return CHECK(IByteSerializable::writeNumber(component.as<double>(), out), "Unable to write lua number");
        }
        case sol::type::boolean:
        {
            return CHECK(IByteSerializable::writeNumber(component.as<bool>(), out), "Unable to write lua bool");
        }
        case sol::type::table:
        {
            return toBinary(component.as<sol::table>(), out, toSerialized);
        }
        case sol::type::userdata:
        case sol::type::lightuserdata:
        {
            const auto&       userData   = component.as<sol::userdata&>();
            const std::string typeString = userData["__type"]["name"];

            const auto& luaTypes = LuaTypeRegistry::getInstance();

            if (!CHECK(luaTypes.contains(typeString), "Unable to serialize unregistered lua user type %s", typeString.c_str()))
                return false;

            if (!CHECK(IByteSerializable::serializeString(typeString, out), "Failed to serialize lua user type's type string"))
                return false;

            return luaTypes.getTypeInfo(typeString).toBinary(userData, out, toSerialized);
        }
        case sol::type::none:
        case sol::type::thread:
        case sol::type::function:
        case sol::type::poly:
        default:
            return CHECK(false, "Unable to serialize lua object - Unsupported type");
        }
    }
}
