#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/MathSerializers.h"

using namespace LibMath;

namespace PantheonCore::Serialization
{
    template <>
    bool toBinary(const Transform& transform, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Vector3) * 2 + sizeof(Quaternion));

        if (!CHECK(IByteSerializable::serializeVector3(transform.getPosition(), out)))
            return false;

        if (!CHECK(IByteSerializable::serializeQuaternion(transform.getRotation(), out)))
            return false;

        if (!CHECK(IByteSerializable::serializeVector3(transform.getScale(), out)))
            return false;

        return true;
    }

    template <>
    size_t fromBinary(Transform& out, const char* data, const size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize transform - Empty buffer"))
            return 0;

        Vector3 position;
        size_t  offset = IByteSerializable::deserializeVector3(position, data, length);

        if (!CHECK(offset != 0, "Unable to deserialize transform position"))
            return 0;

        Quaternion rotation;
        size_t     readBytes = IByteSerializable::deserializeQuaternion(rotation, data + offset, length - offset);

        if (!CHECK(readBytes != 0, "Unable to deserialize transform rotation"))
            return 0;

        offset += readBytes;

        Vector3 scale;
        readBytes = IByteSerializable::deserializeVector3(scale, data + offset, length - offset);

        if (!CHECK(readBytes != 0, "Unable to deserialize transform scale"))
            return 0;

        out.setAll(position, rotation, scale);
        return offset + readBytes;
    }

    template <>
    bool toJson(const Transform& transform, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.Key("position");
        std::string str = transform.getPosition().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.Key("rotation");
        str = transform.getRotation().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.Key("scale");
        str = transform.getScale().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        return writer.EndObject();
    }

    template <>
    bool fromJson(Transform& transform, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize transform - Invalid json object"))
            return false;

        Vector3    position, scale;
        Quaternion rotation;

        auto it = json.FindMember("position");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize transform position"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> position;
        }

        it = json.FindMember("rotation");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize transform rotation"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> rotation;
        }

        it = json.FindMember("scale");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize transform scale"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> scale;
        }

        transform.setAll(position, rotation, scale);
        return true;
    }
}
