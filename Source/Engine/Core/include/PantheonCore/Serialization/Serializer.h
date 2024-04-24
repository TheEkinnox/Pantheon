#pragma once
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace PantheonCore::Serialization
{
    using JsonWriter = rapidjson::Writer<rapidjson::StringBuffer>;
    using JsonValue = rapidjson::Value;

    template <typename T>
    bool toBinary(const T& value, std::vector<char>& out) requires !std::is_enum_v<T>;

    template <typename T>
    size_t fromBinary(T& out, const char* data, size_t length) requires !std::is_enum_v<T>;

    template <typename T>
    bool toBinary(const T& value, std::vector<char>& out) requires std::is_enum_v<T>;

    template <typename T>
    size_t fromBinary(T& out, const char* data, size_t length) requires std::is_enum_v<T>;

    template <typename T>
    bool toJson(const T& value, JsonWriter& writer) requires !std::is_enum_v<T>;

    template <typename T>
    bool fromJson(T& value, const JsonValue& json) requires !std::is_enum_v<T>;

    template <typename T>
    bool toJson(const T& value, JsonWriter& writer) requires std::is_enum_v<T>;

    template <typename T>
    bool fromJson(T& out, const JsonValue& json) requires std::is_enum_v<T>;
}

#include "PantheonCore/Serialization/Serializer.inl"
