#pragma once
#include <vector>

#include <rapidjson/document.h>
#include <rapidjson/writer.h>

namespace PantheonCore::Serialization
{
    template <typename T>
    bool toBinary(const T& value, std::vector<char>& out);

    template <typename T>
    size_t fromBinary(T& out, const char* data, size_t length);

    template <typename T>
    bool toJson(const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer);

    template <typename T>
    bool fromJson(T& value, const rapidjson::Value& json);
}

#include "PantheonCore/Serialization/Serializer.inl"
