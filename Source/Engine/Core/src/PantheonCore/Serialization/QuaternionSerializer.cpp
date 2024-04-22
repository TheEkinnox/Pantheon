#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/MathSerializers.h"

using namespace LibMath;

namespace PantheonCore::Serialization
{
    template <>
    bool toBinary(const Quaternion& quaternion, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::serializeQuaternion(quaternion, out));
    }

    template <>
    size_t fromBinary(Quaternion& out, const char* data, const size_t length)
    {
        const size_t offset = IByteSerializable::deserializeQuaternion(out, data, length);
        return CHECK(offset != 0, "Failed to deserialize Quaternion") ? offset : 0;
    }

    template <>
    bool toJson(const Quaternion& quaternion, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        const std::string str = quaternion.string();
        return CHECK(writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size())), "Failed to serialize Quaternion");
    }

    template <>
    bool fromJson(Quaternion& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsString(), "Unable to deserialize Quaternion - Invalid json value"))
            return false;

        std::istringstream iss(std::string(json.GetString(), json.GetStringLength()));
        iss >> out;

        return true;
    }
}
