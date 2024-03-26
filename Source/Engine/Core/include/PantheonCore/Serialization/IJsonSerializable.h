#pragma once
#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace PantheonCore::Serialization
{
    class IJsonSerializable
    {
    public:
        IJsonSerializable()                               = default;
        IJsonSerializable(const IJsonSerializable& other) = default;
        IJsonSerializable(IJsonSerializable&& other)      = default;
        virtual ~IJsonSerializable()                      = default;

        IJsonSerializable& operator=(const IJsonSerializable& other) = default;
        IJsonSerializable& operator=(IJsonSerializable&& other)      = default;

        /**
         * \brief Serializes the object to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        virtual bool toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const = 0;

        /**
         * \brief Deserializes the object from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        virtual bool fromJson(const rapidjson::Value& json) = 0;
    };
}
