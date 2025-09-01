#include "PantheonRendering/Resources/Material.h"

#include "PantheonRendering/RHI/ITexture.h"

#include <rapidjson/istreamwrapper.h>

using namespace LibMath;
using namespace PantheonCore::Resources;
using namespace PantheonCore::Serialization;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::RHI;

namespace PantheonRendering::Resources
{
    Material::Material(const ResourceRef<IShader>& shader)
    {
        setShader(shader);
    }

    bool Material::load(const std::string& fileName)
    {
        std::ifstream fs(fileName);

        if (!CHECK(fs.is_open(), "Unable to open material file at path \"%s\"", fileName.c_str()))
            return false;

        rapidjson::IStreamWrapper isw(fs);

        rapidjson::Document json;
        json.ParseStream(isw);

        if (!CHECK(!json.HasParseError(), "Unable to parse material from file - Parse error %d", json.GetParseError()))
            return false;

        return fromJson(json);
    }

    bool Material::save(const std::string& fileName) const
    {
        rapidjson::StringBuffer buffer;
        JsonWriter              writer(buffer);

        if (!toJson(writer) || !ASSUME(writer.IsComplete(), "Failed to save material - Generated json is incomplete"))
            return false;

        std::ofstream fs(fileName);

        if (!CHECK(fs.is_open(), "Unable to open material file at path \"%s\"", fileName.c_str()))
            return false;

        fs << std::string_view(buffer.GetString(), buffer.GetLength());

        return CHECK(!fs.bad(), "Failed to write material to \"%s\"", fileName.c_str());
    }

    bool Material::toJson(JsonWriter& writer) const
    {
        writer.StartObject();

        writer.Key("shader");
        if (!CHECK(m_shader.toJson(writer), "Unable to serialize material - Failed to serialize shader"))
            return false;

        writer.Key("properties");
        writer.StartArray();

        for (const auto& [name, property] : m_properties)
        {
            writer.StartObject();

            writer.Key("name");
            writer.String(name.c_str(), static_cast<rapidjson::SizeType>(name.size()));

            writer.Key("type");
            writer.Uint(static_cast<uint8_t>(property.m_type));

            writer.Key("value");

            if (!CHECK(serializePropertyValue(writer, property), "Unable to serialize material property \"%s\"", name.c_str()))
                return false;

            writer.EndObject();
        }

        writer.EndArray();
        return writer.EndObject();
    }

    bool Material::fromJson(const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize material - Json value should be an object"))
            return false;

        auto it = json.FindMember("shader");
        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize material - Missing shader") || !m_shader.fromJson(it->value))
            return false;

        it = json.FindMember("properties");
        return CHECK(it != json.MemberEnd(), "Unable to deserialize material - Missing properties") &&
            deserializeProperties(it->value);
    }

    bool Material::toBinary(std::vector<char>& output) const
    {
        if (!m_shader.toBinary(output))
            return false;

        const ElemCountT propertyCount = static_cast<ElemCountT>(m_properties.size());
        if (!CHECK(writeNumber(propertyCount, output), "Unable to serialize material property count"))
            return false;

        for (const auto& [name, property] : m_properties)
        {
            if (!CHECK(serializeString(name, output), "Unable to serialize material property name"))
                return false;

            if (!serializeProperty(property, output))
                return false;
        }

        return true;
    }

    size_t Material::fromBinary(const char* data, const size_t length)
    {
        m_properties.clear();

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize material - Empty buffer"))
            return 0;

        size_t offset = m_shader.fromBinary(data, length);

        if (offset == 0)
            return 0;

        if (!CHECK(length >= offset, "Unable to deserialize material property count - Invalid offset"))
            return 0;

        ElemCountT propertyCount;
        size_t     readBytes = readNumber(propertyCount, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Failed to read material property count"))
            return 0;

        offset += readBytes;

        for (ElemCountT i = 0; i < propertyCount; ++i)
        {
            if (!CHECK(length >= offset, "Unable to deserialize material property name - Invalid offset"))
                return 0;

            std::string name;
            readBytes = deserializeString(name, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize material property name"))
                return 0;

            offset += readBytes;

            if (!CHECK(length >= offset, "Unable to deserialize material property - Invalid offset"))
                return 0;

            Property property;
            readBytes = deserializeProperty(property, data + offset, length - offset);

            if (readBytes == 0)
                return 0;

            m_properties[name] = property;
            offset += readBytes;
        }

        return offset;
    }

    IShader& Material::getShader() const
    {
        IShader* shader = m_shader.getOrDefault();

        PTH_ASSERT(shader, "Couldn't load shader with key \"%s\" or path \"%s\". Make sure the resource exists",
            m_shader.getKey().c_str(), m_shader.getPath().c_str());

        return *shader;
    }

    void Material::setShader(const ResourceRef<IShader>& shader)
    {
        m_shader = shader;
        m_properties.clear();

        for (const auto& [name, uniform] : m_shader->getUniforms())
        {
            if (name.starts_with(ENGINE_UNIFORM_PREFIX))
                continue;

            m_properties[name] =
            {
                uniform.m_type,
                getDefaultValue(uniform.m_type)
            };
        }
    }

    const Material::Property& Material::getProperty(const std::string& name) const
    {
        const auto it = m_properties.find(name);
        PTH_ASSERT(it != m_properties.end(), "Unable to find material property \"%s\"", name.c_str());
        return it->second;
    }

    Material::Property& Material::getProperty(const std::string& name)
    {
        const auto it = m_properties.find(name);
        PTH_ASSERT(it != m_properties.end(), "Unable to find material property \"%s\"", name.c_str());
        return it->second;
    }

    const std::unordered_map<std::string, Material::Property>& Material::getProperties() const
    {
        return m_properties;
    }

    std::unordered_map<std::string, Material::Property>& Material::getProperties()
    {
        return m_properties;
    }

    void Material::bind() const
    {
        IShader* shader = m_shader.getOrDefault();

        PTH_ASSERT(shader != nullptr, "Failed to bind material - Missing shader");
        shader->bind();

        for (const auto& [name, property] : m_properties)
            bindProperty(shader, name, property);
    }

    std::any Material::getDefaultValue(const EShaderDataType dataType)
    {
        switch (dataType)
        {
        case EShaderDataType::BOOL:
            return false;
        case EShaderDataType::INT:
            return 0;
        case EShaderDataType::UNSIGNED_INT:
            return 0u;
        case EShaderDataType::FLOAT:
            return 0.f;
        case EShaderDataType::VEC2:
            return Vector2();
        case EShaderDataType::VEC3:
            return Vector3();
        case EShaderDataType::VEC4:
            return Vector4();
        case EShaderDataType::MAT3:
            return Matrix3();
        case EShaderDataType::MAT4:
            return Matrix4();
        case EShaderDataType::TEXTURE:
            return ResourceRef<ITexture>();
        case EShaderDataType::UNKNOWN:
        default:
            PTH_ASSERT(false, "Failed to get default value - Unknown data type");
            return {};
        }
    }

    void Material::bindProperty(IShader* shader, const std::string& name, const Property& property)
    {
        PTH_ASSERT(shader, "Unable to bind material property - No shader");

        switch (property.m_type)
        {
        case EShaderDataType::BOOL:
            shader->setUniformInt(name, std::any_cast<bool>(property.m_value));
            break;
        case EShaderDataType::INT:
            shader->setUniformInt(name, std::any_cast<int>(property.m_value));
            break;
        case EShaderDataType::UNSIGNED_INT:
            shader->setUniformUInt(name, std::any_cast<uint32_t>(property.m_value));
            break;
        case EShaderDataType::FLOAT:
            shader->setUniformFloat(name, std::any_cast<float>(property.m_value));
            break;
        case EShaderDataType::VEC2:
            shader->setUniformVec2(name, std::any_cast<const Vector2&>(property.m_value));
            break;
        case EShaderDataType::VEC3:
            shader->setUniformVec3(name, std::any_cast<const Vector3&>(property.m_value));
            break;
        case EShaderDataType::VEC4:
            shader->setUniformVec4(name, std::any_cast<const Vector4&>(property.m_value));
            break;
        case EShaderDataType::MAT3:
            shader->setUniformMat3(name, std::any_cast<const Matrix3&>(property.m_value));
            break;
        case EShaderDataType::MAT4:
            shader->setUniformMat4(name, std::any_cast<const Matrix4&>(property.m_value));
            break;
        case EShaderDataType::TEXTURE:
        {
            shader->setUniformTexture(name, std::any_cast<const ResourceRef<ITexture>&>(property.m_value).getOrDefault());
            break;
        }
        case EShaderDataType::UNKNOWN:
        default:
            PTH_ASSERT(false, "Unknown uniform type");
            return;
        }
    }

    bool Material::serializePropertyValue(JsonWriter& writer, const Property& property)
    {
        switch (property.m_type)
        {
        case EShaderDataType::BOOL:
            return writer.Bool(std::any_cast<bool>(property.m_value));
        case EShaderDataType::INT:
            return writer.Int(std::any_cast<int>(property.m_value));
        case EShaderDataType::UNSIGNED_INT:
            return writer.Uint(std::any_cast<uint32_t>(property.m_value));
        case EShaderDataType::FLOAT:
            return writer.Double(std::any_cast<float>(property.m_value));
        case EShaderDataType::VEC2:
        {
            const std::string str = std::any_cast<Vector2>(property.m_value).string();
            return writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));
        }
        case EShaderDataType::VEC3:
        {
            const std::string str = std::any_cast<Vector3>(property.m_value).string();
            return writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));
        }
        case EShaderDataType::VEC4:
        {
            const std::string str = std::any_cast<Vector4>(property.m_value).string();
            return writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));
        }
        case EShaderDataType::MAT3:
        {
            const std::string str = std::any_cast<Matrix3>(property.m_value).string();
            return writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));
        }
        case EShaderDataType::MAT4:
        {
            const std::string str = std::any_cast<Matrix4>(property.m_value).string();
            return writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));
        }
        case EShaderDataType::TEXTURE:
            return std::any_cast<const ResourceRef<ITexture>&>(property.m_value).toJson(writer);
        case EShaderDataType::UNKNOWN:
        default:
            return false;
        }
    }

    bool Material::deserializeProperties(const JsonValue& json)
    {
        m_properties.clear();

        if (!CHECK(json.IsArray(), "Unable to deserialize material properties - Json value should be an object"))
            return false;

        for (const auto& jsonProperty : json.GetArray())
        {
            auto it = jsonProperty.FindMember("name");
            if (!CHECK(it != jsonProperty.MemberEnd() && it->value.IsString(), "Unable to deserialize material property name"))
                return false;

            std::string name = it->value.GetString();

            Property property;
            it = jsonProperty.FindMember("type");
            if (!CHECK(it != jsonProperty.MemberEnd() && it->value.IsUint(), "Unable to deserialize material property type"))
                return false;

            property.m_type = static_cast<EShaderDataType>(it->value.GetUint());

            it = jsonProperty.FindMember("value");
            if (!deserializePropertyValue(it->value, property))
                return false;

            m_properties[name] = property;
        }

        return true;
    }

    bool Material::deserializePropertyValue(const JsonValue& json, Property& out)
    {
        switch (out.m_type)
        {
        case EShaderDataType::BOOL:
        {
            if (!CHECK(json.IsBool(), "Unable to deserialize boolean material property value"))
                return false;

            out.m_value = json.GetBool();
            return true;
        }
        case EShaderDataType::INT:
        {
            if (!CHECK(json.IsInt(), "Unable to deserialize integer material property value"))
                return false;

            out.m_value = json.GetInt();
            return true;
        }
        case EShaderDataType::UNSIGNED_INT:
        {
            if (!CHECK(json.IsUint(), "Unable to deserialize unsigned integer material property value"))
                return false;

            out.m_value = json.GetUint();
            return true;
        }
        case EShaderDataType::FLOAT:
        {
            if (!CHECK(json.IsFloat(), "Unable to deserialize float material property value"))
                return false;

            out.m_value = json.GetFloat();
            return true;
        }
        case EShaderDataType::VEC2:
        {
            if (!CHECK(json.IsString(), "Unable to deserialize Vector2 material property value"))
                return false;

            std::istringstream iss(json.GetString());

            Vector2 vec;
            iss >> vec;

            out.m_value = vec;
            return true;
        }
        case EShaderDataType::VEC3:
        {
            if (!CHECK(json.IsString(), "Unable to deserialize Vector3 material property value"))
                return false;

            std::istringstream iss(json.GetString());

            Vector3 vec;
            iss >> vec;

            out.m_value = vec;
            return true;
        }
        case EShaderDataType::VEC4:
        {
            if (!CHECK(json.IsString(), "Unable to deserialize Vector4 material property value"))
                return false;

            std::istringstream iss(json.GetString());

            Vector4 vec;
            iss >> vec;

            out.m_value = vec;
            return true;
        }
        case EShaderDataType::MAT3:
        {
            if (!CHECK(json.IsString(), "Unable to deserialize Matrix3 material property value"))
                return false;

            std::istringstream iss(json.GetString());

            Matrix3 vec;
            iss >> vec;

            out.m_value = vec;
            return true;
        }
        case EShaderDataType::MAT4:
        {
            if (!CHECK(json.IsString(), "Unable to deserialize Matrix4 material property value"))
                return false;

            std::istringstream iss(json.GetString());

            Matrix4 vec;
            iss >> vec;

            out.m_value = vec;
            return true;
        }
        case EShaderDataType::TEXTURE:
        {
            ResourceRef<ITexture> texture;

            if (!texture.fromJson(json))
                return false;

            out.m_value = texture;
            return true;
        }
        case EShaderDataType::UNKNOWN:
        default:
            return false;
        }
    }

    bool Material::serializeProperty(const Property& property, std::vector<char>& output)
    {
        size_t offset = output.size();
        output.resize(offset + 1);

        output[offset] = *reinterpret_cast<const char*>(&property.m_type);
        ++offset;

        switch (property.m_type)
        {
        case EShaderDataType::BOOL:
            output.resize(offset + 1);
            output[offset] = *reinterpret_cast<const char*>(std::any_cast<bool>(&property.m_value));
            return true;
        case EShaderDataType::INT:
            return writeNumber(std::any_cast<int>(property.m_value), output);
        case EShaderDataType::UNSIGNED_INT:
            return writeNumber(std::any_cast<uint32_t>(property.m_value), output);
        case EShaderDataType::FLOAT:
            return writeNumber(std::any_cast<float>(property.m_value), output);
        case EShaderDataType::VEC2:
            return serializeVector2(std::any_cast<Vector2>(property.m_value), output);
        case EShaderDataType::VEC3:
            return serializeVector3(std::any_cast<Vector3>(property.m_value), output);
        case EShaderDataType::VEC4:
            return serializeVector4(std::any_cast<Vector4>(property.m_value), output);
        case EShaderDataType::MAT3:
            return serializeMatrix(std::any_cast<Matrix3>(property.m_value), output);
        case EShaderDataType::MAT4:
            return serializeMatrix(std::any_cast<Matrix4>(property.m_value), output);
        case EShaderDataType::TEXTURE:
            return std::any_cast<const ResourceRef<ITexture>&>(property.m_value).toBinary(output);
        case EShaderDataType::UNKNOWN:
        default:
            PTH_ASSERT(false, "Unable to serialize material property - Unknown/Invalid type");
            return false;
        }
    }

    size_t Material::deserializeProperty(Property& out, const char* data, const size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize material property - Empty buffer"))
            return 0;

        out.m_type              = static_cast<EShaderDataType>(data[0]);
        constexpr size_t offset = sizeof(char);

        if (!CHECK(length >= offset, "Unable to deserialize material property - Invalid offset"))
            return 0;

        switch (out.m_type)
        {
        case EShaderDataType::BOOL:
        {
            if (!CHECK(length - offset >= 1, "Unable to deserialize boolean material property - Invalid offset"))
                return 0;

            out.m_value = static_cast<bool>(data[offset]);
            return offset + sizeof(char);
        }
        case EShaderDataType::INT:
        {
            int          val;
            const size_t readBytes = readNumber(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize integer material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::UNSIGNED_INT:
        {
            uint32_t     val;
            const size_t readBytes = readNumber(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize unsigned integer material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::FLOAT:
        {
            float        val;
            const size_t readBytes = readNumber(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize float material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::VEC2:
        {
            Vector2      val;
            const size_t readBytes = deserializeVector2(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize Vector2 material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::VEC3:
        {
            Vector3      val;
            const size_t readBytes = deserializeVector3(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize Vector3 material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::VEC4:
        {
            Vector4      val;
            const size_t readBytes = deserializeVector4(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize Vector4 material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::MAT3:
        {
            Matrix3      val;
            const size_t readBytes = deserializeMatrix(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize Matrix3 material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::MAT4:
        {
            Matrix4      val;
            const size_t readBytes = deserializeMatrix(val, data + offset, length - offset);

            if (!CHECK(readBytes > 0, "Failed to deserialize Matrix4 material property"))
                return 0;

            out.m_value = val;
            return offset + readBytes;
        }
        case EShaderDataType::TEXTURE:
        {
            ResourceRef<ITexture> texture;
            const size_t          readBytes = texture.fromBinary(data + offset, length - offset);

            if (readBytes == 0)
                return 0;

            out.m_value = texture;
            return offset + readBytes;
        }
        case EShaderDataType::UNKNOWN:
        default:
            return 0;
        }
    }
}
