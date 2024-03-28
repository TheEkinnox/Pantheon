#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"

namespace PantheonCore::ECS
{
    struct TagComponent
    {
        std::string m_tag;
    };

    /**
     * \brief Serializes the given tag component into the given byte array
     * \param tag The component to serialize
     * \param output The output memory buffer
     * \return True on success. False otherwise.
     */
    template <>
    bool ComponentRegistry::toBinary<TagComponent>(const TagComponent& tag, std::vector<char>& output, const EntitiesMap&);

    /**
     * \brief Deserializes the tag component from the given memory buffer
     * \param data A pointer to the beginning of the memory buffer
     * \param length The memory buffer's length
     * \return The number of deserialized bytes on success. 0 otherwise.
     */
    template <>
    size_t ComponentRegistry::fromBinary<TagComponent>(TagComponent& out, const char* data, size_t length);

    /**
     * \brief Serializes the tag component to json
     * \param writer The output json writer
     * \return True on success. False otherwise.
     */
    template <>
    bool ComponentRegistry::toJson<TagComponent>(
        const TagComponent& tag, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&);

    /**
     * \brief Deserializes the tag component from json
     * \param json The input json data
     * \return True on success. False otherwise.
     */
    template <>
    bool ComponentRegistry::fromJson<TagComponent>(TagComponent& tag, const rapidjson::Value& json);
}
