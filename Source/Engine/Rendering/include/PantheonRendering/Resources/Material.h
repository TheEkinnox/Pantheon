#pragma once
#include "PantheonRendering/RHI/IShader.h"

#include <PantheonCore/Resources/IResource.h>
#include <PantheonCore/Resources/ResourceRef.h>
#include <PantheonCore/Serialization/IJsonSerializable.h>

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

        /**
         * \brief Creates a default material
         */
        Material() = default;

        /**
         * \brief Creates a material with the given shader
         * \param shader The material's shader
         */
        explicit Material(const PantheonCore::Resources::ResourceRef<RHI::IShader>& shader);

        /**
         * \brief Creates a copy of the given material
         * \param other The material to copy
         */
        Material(const Material& other) = default;

        /**
         * \brief Creates a move copy of the given material
         * \param other The material to move
         */
        Material(Material&& other) noexcept = default;

        /**
         * \brief Destroys the material
         */
        ~Material() override = default;

        /**
         * \brief Assigns a copy of the given material to this one
         * \param other The material to copy
         * \return A reference to the modified material
         */
        Material& operator=(const Material& other) = default;

        /**
         * \brief Moves the given material into this one
         * \param other The material to move
         * \return A reference to the modified material
         */
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
        bool toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the material from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool fromJson(const rapidjson::Value& json) override;

        /**
         * \brief Serializes the material to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the material from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

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

        /**
         * \brief Gets the default value for the given data type
         * \param dataType The values data type
         * \return The data type's default value
         */
        static std::any getDefaultValue(Enums::EShaderDataType dataType);

        /**
         * \brief Binds the given property to the given shader
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
