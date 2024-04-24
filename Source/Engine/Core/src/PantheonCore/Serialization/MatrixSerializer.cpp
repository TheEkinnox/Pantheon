#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/MathSerializers.h"

using namespace LibMath;

namespace PantheonCore::Serialization
{
    template <>
    bool toBinary(const Matrix3& matrix, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::serializeMatrix(matrix, out));
    }

    template <>
    size_t fromBinary(Matrix3& out, const char* data, const size_t length)
    {
        const size_t offset = IByteSerializable::deserializeMatrix(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Matrix3") ? offset : 0;
    }

    template <>
    bool toJson(const Matrix3& matrix, JsonWriter& writer)
    {
        const std::string str = matrix.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Matrix3");
    }

    template <>
    bool fromJson(Matrix3& out, const JsonValue& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Matrix3 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }

    template <>
    bool toBinary(const Matrix4& matrix, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::serializeMatrix(matrix, out));
    }

    template <>
    size_t fromBinary(Matrix4& out, const char* data, const size_t length)
    {
        const size_t offset = IByteSerializable::deserializeMatrix(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Matrix4") ? offset : 0;
    }

    template <>
    bool toJson(const Matrix4& matrix, JsonWriter& writer)
    {
        const std::string str = matrix.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Matrix4");
    }

    template <>
    bool fromJson(Matrix4& out, const JsonValue& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Matrix4 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }
}
