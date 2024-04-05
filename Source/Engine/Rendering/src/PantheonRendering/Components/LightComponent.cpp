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

    bool serializeAmbient(const Core::Light& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::Light));

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out));
    }

    size_t seserializeAmbient(Core::Light& out, const char* data, size_t length)
    {
        const size_t readBytes = IByteSerializable::deserializeVector4(reinterpret_cast<Vector4&>(out.m_color), data, length);
        return CHECK(readBytes > 0, "Unable to deserialize ambient light") ? readBytes : 0;
    }

    bool serializeAmbient(const Core::Light& light, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        const std::string colorString = Vector4(light.m_color).string();
        writer.Key("color");
        writer.String(colorString.c_str(), static_cast<rapidjson::SizeType>(colorString.size()));

        return CHECK(writer.EndObject());
    }

    bool seserializeAmbient(Core::Light& out, const rapidjson::Value& json)
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

    size_t seserializeDirectional(Core::DirectionalLight& out, const char* data, size_t length)
    {
        const size_t offset = IByteSerializable::deserializeVector4(reinterpret_cast<Vector4&>(out.m_color), data, length);

        if (!CHECK(offset > 0, "Unable to deserialize directional light color"))
            return 0;

        if (!CHECK(length >= offset, "Unable to deserialize directional light direction"))
            return 0;

        const size_t dirBytes = IByteSerializable::deserializeVector3(out.m_direction, data + offset, length - offset);
        return CHECK(dirBytes > 0, "Unable to deserialize directional light direction") ? offset + dirBytes : 0;
    }

    bool serializeDirectional(const Core::DirectionalLight& light, rapidjson::Writer<rapidjson::StringBuffer>& writer)
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

    bool seserializeDirectional(Core::DirectionalLight& out, const rapidjson::Value& json)
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

    bool serializeAttenuation(const Core::Attenuation& attenuation, rapidjson::Writer<rapidjson::StringBuffer>& writer)
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

    bool deserializeAttenuation(Core::Attenuation& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize light attenuation - Json value should be an object"))
            return false;

        auto it = json.FindMember("constant");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize light constant attenuation"))
            return false;

        out.m_constant = it->value.Get<float>();

        it = json.FindMember("linear");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize light linear attenuation"))
            return false;

        out.m_linear = it->value.Get<float>();

        it = json.FindMember("quadratic");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize light quadratic attenuation"))
            return false;

        out.m_quadratic = it->value.Get<float>();

        return true;
    }

    bool serializePoint(const Core::PointLight& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::PointLight));

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_position, out))
            && CHECK(IByteSerializable::serializeVector3(reinterpret_cast<const Vector3&>(light.m_attenuation), out));
    }

    size_t deserializePoint(Core::PointLight& out, const char* data, size_t length)
    {
        size_t offset = IByteSerializable::deserializeVector4(reinterpret_cast<Vector4&>(out.m_color), data, length);

        if (!CHECK(offset > 0, "Unable to deserialize point light color"))
            return 0;

        if (!CHECK(length >= offset, "Unable to deserialize point light position"))
            return 0;

        size_t readBytes = IByteSerializable::deserializeVector3(out.m_position, data + offset, length - offset);
        if (!CHECK(readBytes > 0, "Unable to deserialize point light position"))
            return 0;

        offset += readBytes;
        if (!CHECK(length >= offset, "Unable to deserialize point light attenuation"))
            return 0;

        readBytes = IByteSerializable::deserializeVector3(reinterpret_cast<Vector3&>(out.m_attenuation),
            data + offset, length - offset);

        return CHECK(readBytes > 0, "Unable to deserialize point light attenuation") ? offset + readBytes : 0;
    }

    bool serializePoint(const Core::PointLight& light, rapidjson::Writer<rapidjson::StringBuffer>& writer)
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

    bool deserializePoint(Core::PointLight& out, const rapidjson::Value& json)
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

    bool serializeCutoff(const Core::Cutoff& cutoff, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        writer.Key("inner");
        writer.Double(cutoff.m_inner);

        writer.Key("outer");
        writer.Double(cutoff.m_outer);

        return CHECK(writer.EndObject());
    }

    bool deserializeCutoff(Core::Cutoff& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize light cutoff - Json value should be an object"))
            return false;

        auto it = json.FindMember("inner");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize light inner cuttoff"))
            return false;

        out.m_inner = it->value.Get<float>();

        it = json.FindMember("outer");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize light outer cuttoff"))
            return false;

        out.m_outer = it->value.Get<float>();

        return true;
    }

    bool serializeSpot(const Core::SpotLight& light, std::vector<char>& out)
    {
        out.reserve(out.size() + sizeof(Core::SpotLight));

        return CHECK(IByteSerializable::serializeVector4(light.m_color, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_position, out))
            && CHECK(IByteSerializable::serializeVector3(light.m_direction, out))
            && CHECK(IByteSerializable::serializeVector3(reinterpret_cast<const Vector3&>(light.m_attenuation), out))
            && CHECK(IByteSerializable::serializeVector2(reinterpret_cast<const Vector2&>(light.m_cutoff), out));
    }

    size_t deserializeSpot(Core::SpotLight& out, const char* data, size_t length)
    {
        size_t offset = IByteSerializable::deserializeVector4(reinterpret_cast<Vector4&>(out.m_color), data, length);

        if (!CHECK(offset > 0, "Unable to deserialize spot light color"))
            return 0;

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

        readBytes = IByteSerializable::deserializeVector3(reinterpret_cast<Vector3&>(out.m_attenuation),
            data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to deserialize spot light attenuation"))
            return 0;

        offset += readBytes;
        if (!CHECK(length >= offset, "Unable to deserialize spot light cutoff"))
            return 0;

        readBytes = IByteSerializable::deserializeVector2(reinterpret_cast<Vector2&>(out.m_cutoff),
            data + offset, length - offset);

        return CHECK(readBytes > 0, "Unable to deserialize spot light cutoff") ? offset + readBytes : 0;
    }

    bool serializeSpot(const Core::SpotLight& light, rapidjson::Writer<rapidjson::StringBuffer>& writer)
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

    bool deserializeSpot(Core::SpotLight& out, const rapidjson::Value& json)
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

namespace PantheonCore::ECS
{
    using namespace PantheonRendering::Components;

    template <>
    bool ComponentRegistry::toBinary<LightComponent>(const LightComponent& light, std::vector<char>& out, const EntitiesMap&)
    {
        if (!CHECK(Serialization::IByteSerializable::serializeString(lightTypeToString(light.m_type), out)))
            return false;

        switch (light.m_type)
        {
        case ELightType::AMBIENT:
            return serializeAmbient(light.m_ambient, out);
        case ELightType::DIRECTIONAL:
            return serializeDirectional(light.m_directional, out);
        case ELightType::POINT:
            return serializePoint(light.m_point, out);
        case ELightType::SPOT:
            return serializeSpot(light.m_spot, out);
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }

    template <>
    size_t ComponentRegistry::fromBinary<LightComponent>(
        LightComponent& out, const char* data, size_t length)
    {
        std::string  typeString;
        const size_t offset = Serialization::IByteSerializable::deserializeString(typeString, data, length);

        if (!CHECK(offset > 0 && offset <= length, "Unable to deserialize light type string"))
            return 0;

        out.m_type = stringToLightType(typeString);

        switch (out.m_type)
        {
        case ELightType::AMBIENT:
            return seserializeAmbient(out.m_ambient, data + offset, length - offset);
        case ELightType::DIRECTIONAL:
            return seserializeDirectional(out.m_directional, data + offset, length - offset);
        case ELightType::POINT:
            return deserializePoint(out.m_point, data + offset, length - offset);
        case ELightType::SPOT:
            return deserializeSpot(out.m_spot, data + offset, length - offset);
        default:
            return (ASSUME(false, "Unsupported light type"), 0);
        }
    }

    template <>
    bool ComponentRegistry::toJson<LightComponent>(
        const LightComponent& light, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        writer.StartObject();

        const std::string typeString = lightTypeToString(ELightType::AMBIENT);

        writer.Key("type");
        writer.String(typeString.c_str(), static_cast<rapidjson::SizeType>(typeString.size()));

        writer.Key("data");

        switch (light.m_type)
        {
        case ELightType::AMBIENT:
            return serializeAmbient(light.m_ambient, writer) && CHECK(writer.EndObject());
        case ELightType::DIRECTIONAL:
            return serializeDirectional(light.m_directional, writer);
        case ELightType::POINT:
            return serializePoint(light.m_point, writer) && CHECK(writer.EndObject());
        case ELightType::SPOT:
            return serializeSpot(light.m_spot, writer) && CHECK(writer.EndObject());
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }

    template <>
    bool ComponentRegistry::fromJson<LightComponent>(LightComponent& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize light - Json value should be an object"))
            return false;

        auto it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize light - Invalid type string"))
            return false;

        out.m_type = stringToLightType(std::string(it->value.GetString(), it->value.GetStringLength()));

        it = json.FindMember("data");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize light - Light data not found"))
            return false;

        switch (out.m_type)
        {
        case ELightType::AMBIENT:
            return seserializeAmbient(out.m_ambient, it->value);
        case ELightType::DIRECTIONAL:
            return seserializeDirectional(out.m_directional, it->value);
        case ELightType::POINT:
            return deserializePoint(out.m_point, it->value);
        case ELightType::SPOT:
            return deserializeSpot(out.m_spot, it->value);
        default:
            return ASSUME(false, "Unsupported light type") && false;
        }
    }
}
