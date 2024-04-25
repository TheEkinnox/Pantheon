#pragma once
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/Serializer.h"
#include "PantheonCore/Serialization/IByteSerializable.h"

namespace PantheonCore::Serialization
{
    template <typename T>
    bool toBinary(const T& value, std::vector<char>& out) requires (!std::is_enum_v<T>)
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
    size_t fromBinary(T& out, const char* data, const size_t length) requires (!std::is_enum_v<T>)
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
    bool toBinary(const T& value, std::vector<char>& out) requires std::is_enum_v<T>
    {
        return CHECK(IByteSerializable::writeNumber(value, out), "Failed to write \"%s\"", typeid(T).name());
    }

    template <typename T>
    size_t fromBinary(T& out, const char* data, const size_t length) requires std::is_enum_v<T>
    {
        const size_t readBytes = IByteSerializable::readNumber(out, data, length);
        return CHECK(readBytes > 0, "Failed to read \"%s\"", typeid(T).name()) ? readBytes : 0;
    }

    template <typename T>
    bool toJson(const T& value, JsonWriter& writer) requires (!std::is_enum_v<T>)
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
    bool fromJson(T& out, const JsonValue& json) requires (!std::is_enum_v<T>)
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

    template <typename T>
    bool toJson(const T& value, JsonWriter& writer) requires std::is_enum_v<T>
    {
        return CHECK(writer.Uint64(static_cast<uint64_t>(value)), "Failed to write \"%s\"", typeid(T).name());
    }

    template <typename T>
    bool fromJson(T& out, const JsonValue& json) requires std::is_enum_v<T>
    {
        if (!CHECK(json.IsUint64(), "Failed to read \"%s\"", typeid(T).name()))
            return false;

        out = static_cast<T>(json.GetUint64());
        return true;
    }
}
