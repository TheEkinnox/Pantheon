#include "PantheonCore/ECS/Components/TagComponent.h"

#include "PantheonCore/ECS/EntityHandle.h"

using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary<TagComponent>(const TagComponent& tag, std::vector<char>& output, const EntitiesMap&)
    {
        return IByteSerializable::serializeString(tag.m_tag, output);
    }

    template <>
    size_t ComponentRegistry::fromBinary<TagComponent>(TagComponent& out, const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize tag - Empty buffer"))
            return 0;

        const size_t readBytes = IByteSerializable::deserializeString(out.m_tag, data, length);

        if (!CHECK(readBytes > 0, "Unable to deserialize tag"))
            return 0;

        return readBytes;
    }

    template <>
    bool ComponentRegistry::toJson<TagComponent>(
        const TagComponent& tag, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        return CHECK(writer.String(tag.m_tag.c_str(), static_cast<rapidjson::SizeType>(tag.m_tag.size())));
    }

    template <>
    bool ComponentRegistry::fromJson<TagComponent>(TagComponent& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize tag - Json value should be a string"))
            return false;

        out.m_tag = json.GetString();

        return true;
    }
}
