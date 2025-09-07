#include "PantheonCore/ECS/ComponentHandle.h"

using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    bool ComponentHandle::operator==(const ComponentHandle& other) const
    {
        return m_typeId == other.m_typeId && m_owner == other.m_owner;
    }

    ComponentHandle::operator bool() const
    {
        return m_owner && m_owner.getScene()->getStorage(m_typeId).contains(m_owner);
    }

    template <>
    bool ComponentRegistry::toBinary(const ComponentHandle& value, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        return toBinary(value.m_owner, out, toSerialized)
               && value.m_typeId == 0
                   ? IByteSerializable::serializeString("", out)
                   : CHECK(IByteSerializable::serializeString(getInstance().getRegisteredTypeName(value.m_typeId), out),
                       "Failed to write lua component handle's type");
    }

    template <>
    size_t ComponentRegistry::fromBinary(ComponentHandle& out, const char* data, size_t length, Scene* scene)
    {
        if (!CHECK(data && length > 0, "Unable to deserialize lua component handle - Empty buffer"))
            return false;

        const size_t offset = fromBinary(out.m_owner, data, length, scene);

        if (offset == 0)
            return 0;

        if (!CHECK(length >= offset, "Unable to deserialize lua component handle's type - Invalid offset"))
            return 0;

        std::string  typeName;
        const size_t readBytes = IByteSerializable::deserializeString(typeName, data + offset, length - offset);

        if (readBytes == 0)
            return 0;

        if (typeName.empty())
        {
            out.m_typeId = 0;
            return offset + readBytes;
        }

        ComponentRegistry& instance = getInstance();

        if (!CHECK(instance.contains(typeName), "Unable to deserialize lua component handle - Unknown type \"%s\"", typeName.c_str()))
            return 0;

        out.m_typeId = instance.getTypeInfo(typeName).m_typeId;
        return offset + readBytes;
    }

    template <>
    bool ComponentRegistry::toJson(const ComponentHandle& value, JsonWriter& writer, const EntitiesMap& toSerialized)
    {
        writer.StartObject();

        writer.Key("owner");
        if (!toJson(value.m_owner, writer, toSerialized))
            return false;

        writer.Key("type");

        if (value.m_typeId == 0)
            return CHECK(writer.Null() && writer.EndObject(), "Failed to writer lua component handle");

        const ComponentRegistry& instance = getInstance();

        if (!CHECK(instance.contains(value.m_typeId), "Unable to serialize component handle"))
            return false;

        const std::string&        typeName       = instance.getRegisteredTypeName(value.m_typeId);
        const rapidjson::SizeType typeNameLength = static_cast<rapidjson::SizeType>(typeName.size());

        return CHECK(writer.String(typeName.c_str(), typeNameLength), "Failed to write lua component handle's type")
            && CHECK(writer.EndObject(), "Failed to writer lua component handle");
    }

    template <>
    bool ComponentRegistry::fromJson(ComponentHandle& out, const JsonValue& json, Scene* scene)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize lua component handle - Json value should be an object"))
            return false;

        auto it = json.FindMember("owner");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua component handle - Missing owner"))
            return false;

        if (!fromJson(out.m_owner, it->value, scene))
            return false;

        it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize lua component handle - Missing component type"))
            return false;

        if (it->value.IsNull())
        {
            out.m_typeId = 0;
            return true;
        }

        if (!CHECK(it->value.IsString(), "Unable to deserialize component handle type name - Json value should be a string"))
            return false;

        const std::string typeName(it->value.GetString(), it->value.GetStringLength());

        ComponentRegistry& instance = getInstance();

        if (!CHECK(instance.contains(typeName), "Unable to deserialize component handle - Unknown type \"%s\"", typeName.c_str()))
            return false;

        out.m_typeId = instance.getTypeInfo(typeName).m_typeId;
        return true;
    }
}
