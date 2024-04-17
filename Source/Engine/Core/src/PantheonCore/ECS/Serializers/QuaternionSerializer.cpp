#include "PantheonCore/ECS/Serializers/MathSerializers.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(const Quaternion& vector, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::serializeQuaternion(vector, out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(Quaternion& out, const char* data, size_t length, Scene*)
    {
        const size_t offset = IByteSerializable::deserializeQuaternion(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Quaternion") ? offset : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Quaternion& quat, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        const std::string str = quat.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Quaternion");
    }

    template <>
    bool ComponentRegistry::fromJson(Quaternion& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Quaternion - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }
}
