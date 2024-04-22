#pragma once
#include "PantheonCore/Serialization/Serializer.h"

#include <type_traits>

namespace PantheonCore::Serialization
{
    template <typename T>
    bool toBinary(const T& value, std::vector<char>& out) requires std::is_enum_v<T>;

    template <typename T>
    size_t fromBinary(T& out, const char* data, size_t length) requires std::is_enum_v<T>;

    template <typename T>
    bool toJson(const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer) requires std::is_enum_v<T>;

    template <typename T>
    bool fromJson(T& out, const rapidjson::Value& json) requires std::is_enum_v<T>;
}

#include "PantheonCore/Serialization/EnumSerializer.inl"
