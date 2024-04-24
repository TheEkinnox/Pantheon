#include "PantheonCore/ECS/Components/TagComponent.h"

#include "PantheonCore/ECS/EntityHandle.h"

using namespace PantheonCore::ECS;

namespace PantheonCore::Serialization
{
    template <>
    bool toBinary(const TagComponent& tag, std::vector<char>& output)
    {
        return IByteSerializable::serializeString(tag.m_tag, output);
    }

    template <>
    size_t fromBinary(TagComponent& out, const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize tag - Empty buffer"))
            return 0;

        const size_t readBytes = IByteSerializable::deserializeString(out.m_tag, data, length);

        if (!CHECK(readBytes > 0, "Unable to deserialize tag"))
            return 0;

        return readBytes;
    }

    template <>
    bool toJson(const TagComponent& tag, JsonWriter& writer)
    {
        return CHECK(writer.String(tag.m_tag.c_str(), static_cast<rapidjson::SizeType>(tag.m_tag.size())));
    }

    template <>
    bool fromJson(TagComponent& out, const JsonValue& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize tag - Json value should be a string"))
            return false;

        out.m_tag = json.GetString();

        return true;
    }
}
