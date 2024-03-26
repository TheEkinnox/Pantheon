#include "PantheonCore/ECS/Components/TagComponent.h"

#include "PantheonCore/ECS/EntityHandle.h"

using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary<TagComponent>(const TagComponent* tag, std::vector<char>& output, const EntitiesMap&)
    {
        if (!CHECK(tag != nullptr))
            return false;

        return IByteSerializable::serializeString(tag->m_tag, output);
    }

    template <>
    size_t ComponentRegistry::fromBinary<TagComponent>(TagComponent* out, const char* data, size_t length)
    {
        if (!CHECK(out != nullptr, "Unable to deserialize tag - Null output"))
            return 0;

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize tag - Empty buffer"))
            return 0;

        return IByteSerializable::deserializeString(out->m_tag, data, length);
    }

    template <>
    bool ComponentRegistry::toJson<TagComponent>(
        const TagComponent* tag, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        if (!CHECK(tag != nullptr))
            return false;

        return writer.String(tag->m_tag.c_str(), static_cast<rapidjson::SizeType>(tag->m_tag.size()));
    }

    template <>
    bool ComponentRegistry::fromJson<TagComponent>(TagComponent* out, const rapidjson::Value& json)
    {
        if (!CHECK(out != nullptr, "Unable to deserialize tag - Null output"))
            return false;

        if (!CHECK(json.IsString(), "Unable to deserialize tag - Json value should be a string"))
            return false;

        out->m_tag = json.GetString();

        return true;
    }
}
