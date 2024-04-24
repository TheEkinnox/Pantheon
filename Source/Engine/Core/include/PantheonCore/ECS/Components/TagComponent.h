#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"

namespace PantheonCore::ECS
{
    struct TagComponent
    {
        std::string m_tag;
    };
}

namespace PantheonCore::Serialization
{
    /**
     * \brief Serializes the given tag component into the given byte array
     * \param tag The component to serialize
     * \param output The output memory buffer
     * \return True on success. False otherwise.
     */
    template <>
    bool toBinary(const ECS::TagComponent& tag, std::vector<char>& output);

    /**
     * \brief Deserializes the tag component from the given memory buffer
     * \param data A pointer to the beginning of the memory buffer
     * \param length The memory buffer's length
     * \return The number of deserialized bytes on success. 0 otherwise.
     */
    template <>
    size_t fromBinary(ECS::TagComponent& out, const char* data, size_t length);

    /**
     * \brief Serializes the tag component to json
     * \param writer The output json writer
     * \return True on success. False otherwise.
     */
    template <>
    bool toJson(const ECS::TagComponent& tag, JsonWriter& writer);

    /**
     * \brief Deserializes the tag component from json
     * \param json The input json data
     * \return True on success. False otherwise.
     */
    template <>
    bool fromJson(ECS::TagComponent& tag, const JsonValue& json);
}
