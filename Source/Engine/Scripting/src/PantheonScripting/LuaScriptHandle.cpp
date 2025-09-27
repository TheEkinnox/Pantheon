#include "PantheonScripting/LuaScriptHandle.h"

#include "PantheonScripting/LuaScriptList.h"

using namespace PantheonCore::ECS;
using namespace PantheonCore::Serialization;

namespace PantheonScripting
{
    bool LuaScriptHandle::operator<(const LuaScriptHandle& other) const
    {
        return (m_script && other.m_script && m_script->getExecutionOrder() < other.m_script->getExecutionOrder())
            || m_script.getKey().compare(other.m_script.getKey()) < 0
            || m_script.getPath().compare(other.m_script.getPath()) < 0
            || m_owner.getEntity() < other.m_owner.getEntity()
            || std::less<Scene*>{}(m_owner.getScene(), other.m_owner.getScene());
    }

    bool LuaScriptHandle::operator==(const LuaScriptHandle& other) const
    {
        return m_owner == other.m_owner && m_script == other.m_script;
    }

    LuaScriptHandle::operator bool() const
    {
        if (!m_owner || !m_script)
            return false;

        const LuaScriptList* scripts = m_owner.get<LuaScriptList>();
        return scripts->contains(m_script.getKey());
    }
}

namespace PantheonCore::ECS
{
    using namespace PantheonScripting;

    template <>
    bool ComponentRegistry::toBinary(const LuaScriptHandle& value, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        return toBinary(value.m_owner, out, toSerialized)
            && value.m_script.toBinary(out)
            && toBinary(value.m_table, out, toSerialized);
    }

    template <>
    size_t ComponentRegistry::fromBinary(LuaScriptHandle& out, const char* data, const size_t length, Scene* scene)
    {
        if (!CHECK(data && length > 0, "Unable to deserialize lua script handle - Empty buffer"))
            return 0;

        size_t offset = fromBinary(out.m_owner, data, length, scene);

        if (offset == 0)
            return 0;

        if (!CHECK(length >= offset, "Unable to deserialize lua script handle's script - Invalid offset"))
            return 0;

        size_t readBytes = out.m_script.fromBinary(data + offset, length - offset);

        if (readBytes == 0)
            return 0;

        offset += readBytes;

        if (!CHECK(length >= offset, "Unable to deserialize lua script handle's table - Invalid offset"))
            return 0;

        readBytes = fromBinary(out.m_table, data + offset, length - offset, scene);

        return readBytes != 0 ? offset + readBytes : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const LuaScriptHandle& value, JsonWriter& writer, const EntitiesMap& toSerialized)
    {
        writer.StartObject();

        writer.Key("owner");
        if (!toJson(value.m_owner, writer, toSerialized))
            return false;

        writer.Key("script");
        if (!value.m_script.toJson(writer))
            return false;

        writer.Key("table");
        if (!toJson(value.m_table, writer, toSerialized))
            return false;

        return CHECK(writer.EndObject(), "Failed to write lua script handle");
    }

    template <>
    bool ComponentRegistry::fromJson(LuaScriptHandle& out, const JsonValue& json, Scene* scene)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize lua script handle - Json value should be an object"))
            return false;

        auto it = json.FindMember("owner");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua script handle - Missing owner"))
            return false;

        if (!fromJson(out.m_owner, it->value, scene))
            return false;

        it = json.FindMember("script");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua script handle - Missing script"))
            return false;

        if (!out.m_script.fromJson(it->value))
            return false;

        it = json.FindMember("table");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua script handle - Missing table"))
            return false;

        return fromJson(out.m_table, it->value, scene);
    }
}
