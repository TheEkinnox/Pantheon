#include "PantheonCore/ECS/Serializers/MathSerializers.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(const Radian& component, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::writeNumber(component.raw(), out), "Failed to write radian");
    }

    template <>
    size_t ComponentRegistry::fromBinary(Radian& out, const char* data, size_t length, Scene*)
    {
        const size_t readBytes = IByteSerializable::readNumber<Radian, float>(out, data, length);
        return CHECK(readBytes > 0, "Failed to read radian") ? readBytes : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Radian& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        return CHECK(writer.Double(component.raw()), "Failed to write radian");
    }

    template <>
    bool ComponentRegistry::fromJson(Radian& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsNumber(), "Unable to read radian - Json value should be a number"))
            return false;

        out = Radian(json.GetFloat());
        return true;
    }

    template <>
    bool ComponentRegistry::toBinary(const Degree& component, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::writeNumber(component.raw(), out), "Failed to write degrees");
    }

    template <>
    size_t ComponentRegistry::fromBinary(Degree& out, const char* data, size_t length, Scene*)
    {
        const size_t readBytes = IByteSerializable::readNumber<Degree, float>(out, data, length);
        return CHECK(readBytes > 0, "Failed to read degrees") ? readBytes : 0;
    }

    template <>
    bool ComponentRegistry::toJson(const Degree& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        return CHECK(writer.Double(component.raw()), "Failed to write degrees");
    }

    template <>
    bool ComponentRegistry::fromJson(Degree& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsNumber(), "Unable to read degrees - Json value should be a number"))
            return false;

        out = Degree(json.GetFloat());
        return true;
    }
}
