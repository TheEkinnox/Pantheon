#pragma once
#include "PantheonRendering/RHI/IShader.h"

#include <PantheonCore/Resources/IResource.h>
#include <PantheonCore/Resources/ResourceRef.h>
#include <PantheonCore/Serialization/Serializer.h>

#include <any>

namespace PantheonRendering::Resources
{
    class Material final : public PantheonCore::Resources::IResource
    {
        REGISTERED_RESOURCE_BODY()

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
         * \brief Saves the material to the given file
         * \param fileName The target save path
         * \return True if the material was successfully saved. False otherwise.
         */
        bool save(const std::string& fileName) const override;

        /**
         * \brief Serializes the material to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool toJson(PantheonCore::Serialization::JsonWriter& writer) const;

        /**
         * \brief Deserializes the material from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool fromJson(const PantheonCore::Serialization::JsonValue& json);

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
        PantheonCore::Resources::ResourceRef<RHI::IShader> m_shader;
        std::unordered_map<std::string, Property>          m_properties;
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
