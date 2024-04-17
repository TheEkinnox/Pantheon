#include "PantheonCore/ECS/Serializers/MathSerializers.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(const Matrix3& matrix, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::serializeMatrix(matrix, out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(Matrix3& out, const char* data, size_t length, Scene*)
    {
        const size_t offset = IByteSerializable::deserializeMatrix(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Matrix3") ? offset : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Matrix3& matrix, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        const std::string str = matrix.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Matrix3");
    }

    template <>
    bool ComponentRegistry::fromJson(Matrix3& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Matrix3 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }

    template <>
    bool ComponentRegistry::toBinary(const Matrix4& matrix, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::serializeMatrix(matrix, out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(Matrix4& out, const char* data, size_t length, Scene*)
    {
        const size_t offset = IByteSerializable::deserializeMatrix(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Matrix4") ? offset : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Matrix4& matrix, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        const std::string str = matrix.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Matrix4");
    }

    template <>
    bool ComponentRegistry::fromJson(Matrix4& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Matrix4 - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }
}
