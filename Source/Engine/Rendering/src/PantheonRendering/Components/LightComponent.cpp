#include "PantheonRendering/Components/LightComponent.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;
using namespace PantheonRendering::Enums;

namespace PantheonRendering::Components
{
    LightComponent::LightComponent(Core::Light ambient)
        : m_type(ELightType::AMBIENT), m_ambient(std::move(ambient))
    {
    }

    LightComponent::LightComponent(Core::DirectionalLight directional)
        : m_type(ELightType::DIRECTIONAL), m_directional(std::move(directional))
    {
    }

    LightComponent::LightComponent(Core::PointLight point)
        : m_type(ELightType::POINT), m_point(std::move(point))
    {
    }

    LightComponent::LightComponent(Core::SpotLight spot)
        : m_type(ELightType::SPOT), m_spot(std::move(spot))
    {
    }

    bool LightComponent::toBinary(std::vector<char>& out) const
    {
        if (!CHECK(IByteSerializable::writeNumber(m_type, out)))
            return false;

        switch (m_type)
        {
        case ELightType::AMBIENT:
            return serializeAmbient(m_ambient, out);
        case ELightType::DIRECTIONAL:
            return serializeDirectional(m_directional, out);
        case ELightType::POINT:
            return serializePoint(m_point, out);
        case ELightType::SPOT:
            return serializeSpot(m_spot, out);
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }

    size_t LightComponent::fromBinary(const char* data, size_t length)
    {
        const size_t offset = IByteSerializable::readNumber(m_type, data, length);

        if (!CHECK(offset > 0 && offset <= length, "Unable to deserialize light type string"))
            return 0;

        switch (m_type)
        {
        case ELightType::AMBIENT:
            return deserializeAmbient(m_ambient, data + offset, length - offset);
        case ELightType::DIRECTIONAL:
            return deserializeDirectional(m_directional, data + offset, length - offset);
        case ELightType::POINT:
            return deserializePoint(m_point, data + offset, length - offset);
        case ELightType::SPOT:
            return deserializeSpot(m_spot, data + offset, length - offset);
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }

    bool LightComponent::toJson(JsonWriter& writer) const
    {
        writer.StartObject();

        const std::string typeString = lightTypeToString(m_type);

        writer.Key("type");
        writer.String(typeString.c_str(), static_cast<rapidjson::SizeType>(typeString.size()));

        writer.Key("data");

        switch (m_type)
        {
        case ELightType::AMBIENT:
            return serializeAmbient(m_ambient, writer) && CHECK(writer.EndObject());
        case ELightType::DIRECTIONAL:
            return serializeDirectional(m_directional, writer);
        case ELightType::POINT:
            return serializePoint(m_point, writer) && CHECK(writer.EndObject());
        case ELightType::SPOT:
            return serializeSpot(m_spot, writer) && CHECK(writer.EndObject());
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }

    bool LightComponent::fromJson(const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize light - Json value should be an object"))
            return false;

        auto it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize light - Invalid type string"))
            return false;

        m_type = stringToLightType(std::string(it->value.GetString(), it->value.GetStringLength()));

        it = json.FindMember("data");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize light - Light data not found"))
            return false;

        switch (m_type)
        {
        case ELightType::AMBIENT:
            return deserializeAmbient(m_ambient, it->value);
        case ELightType::DIRECTIONAL:
            return deserializeDirectional(m_directional, it->value);
        case ELightType::POINT:
            return deserializePoint(m_point, it->value);
        case ELightType::SPOT:
            return deserializeSpot(m_spot, it->value);
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }

    bool serializeAmbient(const Core::Light& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::Light));

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out));
    }

    size_t deserializeAmbient(Core::Light& out, const char* data, size_t length)
    {
        Vector4      colorVec4;
        const size_t readBytes = IByteSerializable::deserializeVector4(colorVec4, data, length);

        if (!CHECK(readBytes > 0, "Unable to deserialize ambient light"))
            return 0;

        out.m_color = colorVec4;
        return readBytes;
    }

    bool serializeAmbient(const Core::Light& light, JsonWriter& writer)
    {
        writer.StartObject();

        const std::string colorString = Vector4(light.m_color).string();
        writer.Key("color");
        writer.String(colorString.c_str(), static_cast<rapidjson::SizeType>(colorString.size()));

        return CHECK(writer.EndObject());
    }

    bool deserializeAmbient(Core::Light& out, const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize ambient light - Json value should be an object"))
            return false;

        const auto it = json.FindMember("color");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize ambient light color"))
            return false;

        std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));

        Vector4 colorVec;
        iss >> colorVec;
        out.m_color = colorVec;

        return true;
    }

    bool serializeDirectional(const Core::DirectionalLight& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::DirectionalLight));

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_direction, out ));
    }

    size_t deserializeDirectional(Core::DirectionalLight& out, const char* data, size_t length)
    {
        Vector4      colorVec4;
        const size_t offset = IByteSerializable::deserializeVector4(colorVec4, data, length);

        if (!CHECK(offset > 0, "Unable to deserialize directional light color"))
            return 0;

        out.m_color = colorVec4;

        if (!CHECK(length >= offset, "Unable to deserialize directional light direction"))
            return 0;

        const size_t dirBytes = IByteSerializable::deserializeVector3(out.m_direction, data + offset, length - offset);
        return CHECK(dirBytes > 0, "Unable to deserialize directional light direction") ? offset + dirBytes : 0;
    }

    bool serializeDirectional(const Core::DirectionalLight& light, JsonWriter& writer)
    {
        writer.StartObject();

        std::string vecString = Vector4(light.m_color).string();
        writer.Key("color");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        vecString = light.m_direction.string();
        writer.Key("direction");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        return CHECK(writer.EndObject());
    }

    bool deserializeDirectional(Core::DirectionalLight& out, const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize directional light - Json value should be an object"))
            return false;

        auto it = json.FindMember("color");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize directional light color"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));

            Vector4 colorVec;
            iss >> colorVec;
            out.m_color = colorVec;
        }

        it = json.FindMember("direction");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize directional light direction"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> out.m_direction;
        }

        return true;
    }

    bool serializeAttenuation(const Core::Attenuation& attenuation, JsonWriter& writer)
    {
        writer.StartObject();

        writer.Key("constant");
        writer.Double(attenuation.m_constant);

        writer.Key("linear");
        writer.Double(attenuation.m_linear);

        writer.Key("quadratic");
        writer.Double(attenuation.m_quadratic);

        return CHECK(writer.EndObject());
    }

    bool deserializeAttenuation(Core::Attenuation& out, const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize light attenuation - Json value should be an object"))
            return false;

        auto it = json.FindMember("constant");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize light constant attenuation"))
            return false;

        out.m_constant = it->value.Get<float>();

        it = json.FindMember("linear");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize light linear attenuation"))
            return false;

        out.m_linear = it->value.Get<float>();

        it = json.FindMember("quadratic");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize light quadratic attenuation"))
            return false;

        out.m_quadratic = it->value.Get<float>();

        return true;
    }

    bool serializePoint(const Core::PointLight& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::PointLight));
        const Vector3 attenuationVec(light.m_attenuation.m_constant, light.m_attenuation.m_linear, light.m_attenuation.m_quadratic);

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_position, out))
            && CHECK(IByteSerializable::serializeVector3(attenuationVec, out));
    }

    size_t deserializePoint(Core::PointLight& out, const char* data, size_t length)
    {
        Vector4 colorVec4;
        size_t  offset = IByteSerializable::deserializeVector4(colorVec4, data, length);

        if (!CHECK(offset > 0, "Unable to deserialize point light color"))
            return 0;

        out.m_color = colorVec4;

        if (!CHECK(length >= offset, "Unable to deserialize point light position"))
            return 0;

        size_t readBytes = IByteSerializable::deserializeVector3(out.m_position, data + offset, length - offset);
        if (!CHECK(readBytes > 0, "Unable to deserialize point light position"))
            return 0;

        offset += readBytes;
        if (!CHECK(length >= offset, "Unable to deserialize point light attenuation"))
            return 0;

        Vector3 attenuation;
        readBytes = IByteSerializable::deserializeVector3(attenuation, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to deserialize point light attenuation"))
            return 0;

        out.m_attenuation = { attenuation.m_x, attenuation.m_y, attenuation.m_z };

        return offset + readBytes;
    }

    bool serializePoint(const Core::PointLight& light, JsonWriter& writer)
    {
        writer.StartObject();

        std::string vecString = Vector4(light.m_color).string();
        writer.Key("color");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        vecString = light.m_position.string();
        writer.Key("position");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        writer.Key("attenuation");

        if (!serializeAttenuation(light.m_attenuation, writer))
            return false;

        return CHECK(writer.EndObject());
    }

    bool deserializePoint(Core::PointLight& out, const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize point light - Json value should be an object"))
            return false;

        auto it = json.FindMember("color");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize point light color"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));

            Vector4 colorVec;
            iss >> colorVec;
            out.m_color = colorVec;
        }

        it = json.FindMember("position");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize point light position"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> out.m_position;
        }

        it = json.FindMember("attenuation");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize point light - attenuation not found"))
            return false;

        return deserializeAttenuation(out.m_attenuation, it->value);
    }

    bool serializeCutoff(const Core::Cutoff& cutoff, JsonWriter& writer)
    {
        writer.StartObject();

        writer.Key("inner");
        writer.Double(cutoff.m_inner);

        writer.Key("outer");
        writer.Double(cutoff.m_outer);

        return CHECK(writer.EndObject());
    }

    bool deserializeCutoff(Core::Cutoff& out, const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize light cutoff - Json value should be an object"))
            return false;

        auto it = json.FindMember("inner");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize light inner cuttoff"))
            return false;

        out.m_inner = it->value.Get<float>();

        it = json.FindMember("outer");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize light outer cuttoff"))
            return false;

        out.m_outer = it->value.Get<float>();

        return true;
    }

    bool serializeSpot(const Core::SpotLight& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::SpotLight));

        const Vector3 attenuation(light.m_attenuation.m_constant, light.m_attenuation.m_linear, light.m_attenuation.m_quadratic);
        const Vector2 cutoff(light.m_cutoff.m_inner, light.m_cutoff.m_outer);

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_position, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_direction, out))
            && CHECK(IByteSerializable::serializeVector3(attenuation, out))
            && CHECK(IByteSerializable::serializeVector2(cutoff, out));
    }

    size_t deserializeSpot(Core::SpotLight& out, const char* data, size_t length)
    {
        Vector4 colorVec4;
        size_t  offset = IByteSerializable::deserializeVector4(colorVec4, data, length);

        if (!CHECK(offset > 0, "Unable to deserialize spot light color"))
            return 0;

        out.m_color = colorVec4;

        if (!CHECK(length >= offset, "Unable to deserialize spot light position"))
            return 0;

        size_t readBytes = IByteSerializable::deserializeVector3(out.m_position, data + offset, length - offset);
        if (!CHECK(readBytes > 0, "Unable to deserialize spot light position"))
            return 0;

        offset += readBytes;
        if (!CHECK(length >= offset, "Unable to deserialize spot light direction"))
            return 0;

        readBytes = IByteSerializable::deserializeVector3(out.m_direction, data + offset, length - offset);
        if (!CHECK(readBytes > 0, "Unable to deserialize spot light direction"))
            return 0;

        offset += readBytes;
        if (!CHECK(length >= offset, "Unable to deserialize spot light attenuation"))
            return 0;

        Vector3 attenuation;
        readBytes = IByteSerializable::deserializeVector3(attenuation, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to deserialize spot light attenuation"))
            return 0;

        out.m_attenuation = { attenuation.m_x, attenuation.m_y, attenuation.m_z };

        offset += readBytes;
        if (!CHECK(length >= offset, "Unable to deserialize spot light cutoff"))
            return 0;

        Vector2 cutoff;
        readBytes = IByteSerializable::deserializeVector2(cutoff, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to deserialize spot light cutoff"))
            return 0;

        out.m_cutoff = { cutoff.m_x, cutoff.m_y };

        return offset + readBytes;
    }

    bool serializeSpot(const Core::SpotLight& light, JsonWriter& writer)
    {
        writer.StartObject();

        std::string vecString = Vector4(light.m_color).string();
        writer.Key("color");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        vecString = light.m_position.string();
        writer.Key("position");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        vecString = light.m_direction.string();
        writer.Key("direction");
        writer.String(vecString.c_str(), static_cast<rapidjson::SizeType>(vecString.size()));

        writer.Key("attenuation");

        if (!serializeAttenuation(light.m_attenuation, writer))
            return false;

        writer.Key("cutoff");

        if (!serializeCutoff(light.m_cutoff, writer))
            return false;

        return CHECK(writer.EndObject());
    }

    bool deserializeSpot(Core::SpotLight& out, const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize spot light - Json value should be an object"))
            return false;

        auto it = json.FindMember("color");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize spot light color"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));

            Vector4 colorVec;
            iss >> colorVec;
            out.m_color = colorVec;
        }

        it = json.FindMember("position");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize spot light position"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> out.m_position;
        }

        it = json.FindMember("direction");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize spot light direction"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> out.m_direction;
        }

        it = json.FindMember("attenuation");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize spot light - attenuation not found"))
            return false;

        if (!deserializeAttenuation(out.m_attenuation, it->value))
            return false;

        it = json.FindMember("cutoff");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize spot light - cutoff not found"))
            return false;

        return deserializeCutoff(out.m_cutoff, it->value);
    }
}
