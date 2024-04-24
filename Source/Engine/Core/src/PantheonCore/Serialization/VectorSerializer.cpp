#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/MathSerializers.h"

using namespace LibMath;

namespace PantheonCore::Serialization
{
    template <>
    bool toBinary(const Vector2& vector, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::serializeVector2(vector, out));
    }

    template <>
    size_t fromBinary(Vector2& out, const char* data, size_t length)
    {
        const size_t offset = IByteSerializable::deserializeVector2(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Vector2") ? offset : 0;
    }

    template <>
    bool toJson(const Vector2& vector, JsonWriter& writer)
    {
        const std::string str = vector.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Vector2");
    }

    template <>
    bool fromJson(Vector2& out, const JsonValue& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Vector2 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }

    template <>
    bool toBinary(const Vector3& vector, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::serializeVector3(vector, out));
    }

    template <>
    size_t fromBinary(Vector3& out, const char* data, size_t length)
    {
        const size_t offset = IByteSerializable::deserializeVector3(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Vector3") ? offset : 0;
    }

    template <>
    bool toJson(const Vector3& vector, JsonWriter& writer)
    {
        const std::string str = vector.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Vector3");
    }

    template <>
    bool fromJson(Vector3& out, const JsonValue& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Vector3 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }

    template <>
    bool toBinary(const Vector4& vector, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::serializeVector4(vector, out));
    }

    template <>
    size_t fromBinary(Vector4& out, const char* data, size_t length)
    {
        const size_t offset = IByteSerializable::deserializeVector4(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Vector4") ? offset : 0;
    }

    template <>
    bool toJson(const Vector4& vector, JsonWriter& writer)
    {
        const std::string str = vector.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Vector4");
    }

    template <>
    bool fromJson(Vector4& out, const JsonValue& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Vector4 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }
}
