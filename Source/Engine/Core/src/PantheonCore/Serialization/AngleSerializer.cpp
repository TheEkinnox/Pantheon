#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/MathSerializers.h"

using namespace LibMath;

namespace PantheonCore::Serialization
{
    template <>
    bool toBinary(const Radian& angle, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::writeNumber(angle.raw(), out), "Failed to write radian");
    }

    template <>
    size_t fromBinary(Radian& out, const char* data, const size_t length)
    {
        const size_t readBytes = IByteSerializable::readNumber<Radian, float>(out, data, length);
        return CHECK(readBytes > 0, "Failed to read radian") ? readBytes : 0;
    }

    template <>
    bool toJson(const Radian& angle, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        return CHECK(writer.Double(angle.raw()), "Failed to write radian");
    }

    template <>
    bool fromJson(Radian& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsNumber(), "Unable to read radian - Json value should be a number"))
            return false;

        out = Radian(json.GetFloat());
        return true;
    }

    template <>
    bool toBinary(const Degree& angle, std::vector<char>& out)
    {
        return CHECK(IByteSerializable::writeNumber(angle.raw(), out), "Failed to write degrees");
    }

    template <>
    size_t fromBinary(Degree& out, const char* data, const size_t length)
    {
        const size_t readBytes = IByteSerializable::readNumber<Degree, float>(out, data, length);
        return CHECK(readBytes > 0, "Failed to read degrees") ? readBytes : 0;
    }

    template <>
    bool toJson(const Degree& angle, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        return CHECK(writer.Double(angle.raw()), "Failed to write degrees");
    }

    template <>
    bool fromJson(Degree& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsNumber(), "Unable to read degrees - Json value should be a number"))
            return false;

        out = Degree(json.GetFloat());
        return true;
    }
}
