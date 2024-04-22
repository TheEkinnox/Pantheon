#pragma once
#include "PantheonCore/Serialization/Serializer.h"

namespace PantheonCore::Serialization
{
    template <typename T>
    bool toBinary(const T& value, std::vector<char>& out)
    {
        constexpr bool hasToBinary = requires
        {
            value.toBinary(out);
        };

        if constexpr (hasToBinary)
            return value.toBinary(out);
        else
            return ASSUME(false, "Binary serialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    size_t fromBinary(T& out, const char* data, const size_t length)
    {
        constexpr bool hasFromBinary = requires
        {
            out.fromBinary(data, length);
        };

        if constexpr (hasFromBinary)
            return out.fromBinary(data, length);
        else
            return ((void)ASSUME(false, "Binary deserialization is not defined for \"%s\"", typeid(T).name()), 0);
    }

    template <typename T>
    bool toJson(const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        constexpr bool hasToJson = requires
        {
            value.toJson(writer);
        };

        if constexpr (hasToJson)
            return value.toJson(writer);
        else
            return ASSUME(false, "Json serialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    bool fromJson(T& out, const rapidjson::Value& json)
    {
        constexpr bool hasFromJson = requires
        {
            out.fromJson(json);
        };

        if constexpr (hasFromJson)
            return out.fromJson(json);
        else
            return ASSUME(false, "Json deserialization is not defined for \"%s\"", typeid(T).name()) && false;
    }
}
