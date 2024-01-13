#pragma once
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace PantheonCore::Serialization
{
    class IJsonSerializable
    {
    public:
        virtual ~IJsonSerializable() = default;

        /**
         * \brief Serializes the object to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        virtual bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;

        /**
         * \brief Deserializes the object from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        virtual bool deserialize(const rapidjson::Value& json) = 0;
    };
}
