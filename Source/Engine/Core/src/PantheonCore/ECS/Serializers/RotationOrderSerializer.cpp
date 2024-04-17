#include "PantheonCore/ECS/Serializers/MathSerializers.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(const ERotationOrder& component, std::vector<char>& out, const EntitiesMap&)
    {
        return CHECK(IByteSerializable::writeNumber(static_cast<uint8_t>(component), out), "Failed to write rotation order");
    }

    template <>
    size_t ComponentRegistry::fromBinary(ERotationOrder& out, const char* data, size_t length, Scene*)
    {
        const size_t readBytes = IByteSerializable::readNumber<ERotationOrder, uint8_t>(out, data, length);
        return CHECK(readBytes > 0, "Failed to read rotation order") ? readBytes : 0;
    }

    template <>
    bool ComponentRegistry::toJson(
        const ERotationOrder& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        return CHECK(writer.Uint(static_cast<unsigned>(component)), "Failed to write rotation order");
    }

    template <>
    bool ComponentRegistry::fromJson(ERotationOrder& out, const rapidjson::Value& json, Scene*)
    {
        if (!CHECK(json.IsUint(), "Unable to read rotation order - Json value should be an unsigned integer"))
            return false;

        out = static_cast<ERotationOrder>(json.GetUint());
        return true;
    }
}
