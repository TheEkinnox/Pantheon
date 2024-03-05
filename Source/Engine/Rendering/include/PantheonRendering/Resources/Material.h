#pragma once
#include "PantheonRendering/Core/Color.h"
#include "PantheonRendering/RHI/ITexture.h"
#include "PantheonRendering/RHI/IShader.h"

#include <PantheonCore/Resources/IResource.h>
#include <PantheonCore/Resources/ResourceRef.h>
#include <PantheonCore/Serialization/IJsonSerializable.h>

#include <Vector/Vector2.h>

#include <any>

namespace PantheonRendering::Resources
{
    class Material final : public PantheonCore::Resources::IResource, public PantheonCore::Serialization::IJsonSerializable
    {
        REGISTERED_RESOURCE_BODY(Material)

    public:
        struct Property
        {
            Enums::EShaderDataType m_type;
            std::any               m_value;
        };

        Material() = default;
        explicit Material(const PantheonCore::Resources::ResourceRef<RHI::IShader>& shader);

        Material(const Material& other)     = default;
        Material(Material&& other) noexcept = default;
        ~Material() override                = default;

        Material& operator=(const Material& other)     = default;
        Material& operator=(Material&& other) noexcept = default;

        /**
         * \brief Loads the material from the given file
         * \param fileName The path of the material to load
         * \return True if the material was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        bool init() override
        {
            return true;
        }

        /**
         * \brief Serializes the material to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the material from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool deserialize(const rapidjson::Value& json) override;

        /**
         * \brief Serializes the material to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the material from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

        /**
         * \brief Gets the material's shader
         * \return A reference to the material's shader
         */
        RHI::IShader& getShader() const;

        /**
         * \brief Sets the material's shader to the given value
         * \param shader The material's new shader's resource reference
         */
        void setShader(const PantheonCore::Resources::ResourceRef<RHI::IShader>& shader);

        /**
         * \brief Gets read-only access to the property with the given name
         * \param name The target property's name
         * \return The found property
         */
        const Property& getProperty(const std::string& name) const;

        /**
         * \brief Gets read & write access to the property with the given name
         * \param name The target property's name
         * \return The found property
         */
        Property& getProperty(const std::string& name);

        /**
         * \brief Gets read-only access to the material's properties
         * \return The material's properties
         */
        const std::unordered_map<std::string, Property>& getProperties() const;

        /**
         * \brief Gets read & write access to the material's properties
         * \return The material's properties
         */
        std::unordered_map<std::string, Property>& getProperties();

        /**
         * \brief Gets read-only access to the property with the given name
         * \param name The target property's name
         * \return The found property
         */
        template <typename T>
        const T& getProperty(const std::string& name) const;

        /**
         * \brief Gets read & write access to the property with the given name
         * \param name The target property's name
         * \return The found property
         */
        template <typename T>
        T& getProperty(const std::string& name);

        /**
         * \brief Binds the material to the current context
         */
        void bind() const;

    private:
        static constexpr const char* ENGINE_UNIFORM_PREFIX = "pth_";

        PantheonCore::Resources::ResourceRef<RHI::IShader> m_shader;
        std::unordered_map<std::string, Property>          m_properties;

        static std::any getDefaultValue(Enums::EShaderDataType dataType);

        /**
         * \brief
         * \param shader The target shader
         * \param name The target property's name
         * \param property The target property
         */
        static void bindProperty(RHI::IShader* shader, const std::string& name, const Property& property);

        /**
         * \brief Serializes the material to json
         * \param writer The output json writer
         * \param property The property to serialize
         * \return True on success. False otherwise.
         */
        static bool serializePropertyValue(rapidjson::Writer<rapidjson::StringBuffer>& writer, const Property& property);

        /**
         * \brief Deserializes the material's properties from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool deserializeProperties(const rapidjson::Value& json);

        /**
         * \brief Deserializes the material from json
         * \param json The input json data
         * \param out The output property
         * \return True on success. False otherwise.
         */
        static bool deserializePropertyValue(const rapidjson::Value& json, Property& out);

        /**
         * \brief Writes the given property into the given memory buffer
         * \param property The property to serialize
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        static bool serializeProperty(const Property& property, std::vector<char>& output);
    };

    template <typename T>
    const T& Material::getProperty(const std::string& name) const
    {
        return std::any_cast<const T&>(getProperty(name).m_value);
    }

    template <typename T>
    T& Material::getProperty(const std::string& name)
    {
        return std::any_cast<T&>(getProperty(name).m_value);
    }
}
