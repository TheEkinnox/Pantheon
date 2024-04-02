#pragma once
#include "PantheonRendering/Enums/ELightType.h"
#include "PantheonRendering/Core/Light.h"

#include <PantheonCore/ECS/ComponentRegistry.h>

namespace PantheonRendering::Components
{
    struct LightComponent
    {
        Enums::ELightType m_type;

        Core::Light            m_ambient;
        Core::DirectionalLight m_directional;
        Core::PointLight       m_point;
        Core::SpotLight        m_spot;

        LightComponent(Core::Light ambient = Core::Light());
        LightComponent(Core::DirectionalLight directional);
        LightComponent(Core::PointLight point);
        LightComponent(Core::SpotLight spot);

        LightComponent(const LightComponent& other)     = default;
        LightComponent(LightComponent&& other) noexcept = default;
        ~LightComponent()                               = default;

        LightComponent& operator=(const LightComponent& other)     = default;
        LightComponent& operator=(LightComponent&& other) noexcept = default;
    };

    /**
     * \brief Serializes the given ambient light to a byte array
     * \param light The light to serialize
     * \param out The output memory buffer
     * \return True on success. False otherwise
     */
    bool SerializeAmbient(const Core::Light& light, std::vector<char>& out);

    /**
     * \brief Deserializes the given ambient light from the given memory buffer
     * \param out The output light
     * \param data A pointer to the beginning of the memory buffer
     * \param length The memory buffer's length
     * \return The number of deserialized bytes on success. 0 otherwise.
     */
    size_t DeserializeAmbient(Core::Light& out, const char* data, size_t length);

    /**
     * \brief Serializes the given ambient light to json
     * \param light The light to serialize
     * \param writer The output json writer
     * \return True on success. False otherwise
     */
    bool SerializeAmbient(const Core::Light& light, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /**
     * \brief Deserializes the given ambient light from json
     * \param out The output light
     * \param json The input json data
     * \return True on success. False otherwise
     */
    bool DeserializeAmbient(Core::Light& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given directional light to a byte array
     * \param light The light to serialize
     * \param out The output memory buffer
     * \return True on success. False otherwise
     */
    bool SerializeDirectional(const Core::DirectionalLight& light, std::vector<char>& out);

    /**
     * \brief Deserializes the given directional light from the given memory buffer
     * \param out The output light
     * \param data A pointer to the beginning of the memory buffer
     * \param length The memory buffer's length
     * \return The number of deserialized bytes on success. 0 otherwise.
     */
    size_t DeserializeDirectional(Core::DirectionalLight& out, const char* data, size_t length);

    /**
     * \brief Serializes the given directional light to json
     * \param light The light to serialize
     * \param writer The output json writer
     * \return True on success. False otherwise
     */
    bool SerializeDirectional(const Core::DirectionalLight& light, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /**
     * \brief Deserializes the given directional light from json
     * \param out The output light
     * \param json The input json data
     * \return True on success. False otherwise
     */
    bool DeserializeDirectional(Core::DirectionalLight& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given attenuation data to json
     * \param attenuation The attenuation data to serialize
     * \param writer The output json writer
     * \return True on success. False otherwise
     */
    bool SerializeAttenuation(const Core::Attenuation& attenuation, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /**
     * \brief Deserializes the given attenuation data from json
     * \param out The output attenuation data
     * \param json The input json data
     * \return True on success. False otherwise
     */
    bool DeserializeAttenuation(Core::Attenuation& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given point light to a byte array
     * \param light The light to serialize
     * \param out The output memory buffer
     * \return True on success. False otherwise
     */
    bool SerializePoint(const Core::PointLight& light, std::vector<char>& out);

    /**
     * \brief Deserializes the given point light from the given memory buffer
     * \param out The output light
     * \param data A pointer to the beginning of the memory buffer
     * \param length The memory buffer's length
     * \return The number of deserialized bytes on success. 0 otherwise.
     */
    size_t DeserializePoint(Core::PointLight& out, const char* data, size_t length);

    /**
     * \brief Serializes the given point light to json
     * \param light The light to serialize
     * \param writer The output json writer
     * \return True on success. False otherwise
     */
    bool SerializePoint(const Core::PointLight& light, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /**
     * \brief Deserializes the given point light from json
     * \param out The output light
     * \param json The input json data
     * \return True on success. False otherwise
     */
    bool DeserializePoint(Core::PointLight& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given cutoff data to json
     * \param cutoff The cutoff data to serialize
     * \param writer The output json writer
     * \return True on success. False otherwise
     */
    bool SerializeCutoff(const Core::Cutoff& cutoff, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /**
     * \brief Deserializes the given cutoff data from json
     * \param out The output cutoff data
     * \param json The input json data
     * \return True on success. False otherwise
     */
    bool DeserializeCutoff(Core::Cutoff& out, const rapidjson::Value& json);

    /**
     * \brief Serializes the given spot light to a byte array
     * \param light The light to serialize
     * \param out The output memory buffer
     * \return True on success. False otherwise
     */
    bool SerializeSpot(const Core::SpotLight& light, std::vector<char>& out);

    /**
     * \brief Deserializes the given spot light from the given memory buffer
     * \param out The output light
     * \param data A pointer to the beginning of the memory buffer
     * \param length The memory buffer's length
     * \return The number of deserialized bytes on success. 0 otherwise.
     */
    size_t DeserializeSpot(Core::SpotLight& out, const char* data, size_t length);

    /**
     * \brief Serializes the given spot light to json
     * \param light The light to serialize
     * \param writer The output json writer
     * \return True on success. False otherwise
     */
    bool SerializeSpot(const Core::SpotLight& light, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    /**
     * \brief Deserializes the given spot light from json
     * \param out The output light
     * \param json The input json data
     * \return True on success. False otherwise
     */
    bool DeserializeSpot(Core::SpotLight& out, const rapidjson::Value& json);
}

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary<PantheonRendering::Components::LightComponent>(
        const PantheonRendering::Components::LightComponent&, std::vector<char>&, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary<PantheonRendering::Components::LightComponent>(
        PantheonRendering::Components::LightComponent&, const char*, size_t);

    template <>
    bool ComponentRegistry::toJson<PantheonRendering::Components::LightComponent>(
        const PantheonRendering::Components::LightComponent&, rapidjson::Writer<rapidjson::StringBuffer>&, const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson<PantheonRendering::Components::LightComponent>(
        PantheonRendering::Components::LightComponent&, const rapidjson::Value&);
}
