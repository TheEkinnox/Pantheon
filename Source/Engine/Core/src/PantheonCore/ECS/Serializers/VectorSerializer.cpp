#include "PantheonCore/ECS/Serializers/MathSerializers.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(const Vector2& vector, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::serializeVector2(vector, out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(Vector2& out, const char* data, size_t length, Scene*)
    {
        const size_t offset = IByteSerializable::deserializeVector2(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Vector2") ? offset : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Vector2& vector, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        const std::string str = vector.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Vector2");
    }

    template <>
    bool ComponentRegistry::fromJson(Vector2& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Vector2 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }

    template <>
    bool ComponentRegistry::toBinary(const Vector3& vector, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::serializeVector3(vector, out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(Vector3& out, const char* data, size_t length, Scene*)
    {
        const size_t offset = IByteSerializable::deserializeVector3(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Vector3") ? offset : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Vector3& vector, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        const std::string str = vector.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Vector3");
    }

    template <>
    bool ComponentRegistry::fromJson(Vector3& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Vector3 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }

    template <>
    bool ComponentRegistry::toBinary(const Vector4& vector, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::serializeVector4(vector, out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(Vector4& out, const char* data, size_t length, Scene*)
    {
        const size_t offset = IByteSerializable::deserializeVector4(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Vector4") ? offset : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Vector4& vector, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        const std::string str = vector.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Vector4");
    }

    template <>
    bool ComponentRegistry::fromJson(Vector4& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Vector4 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }
}
