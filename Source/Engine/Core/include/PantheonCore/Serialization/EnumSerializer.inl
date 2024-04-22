#pragma once
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/EnumSerializer.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/TypeTraits.h"

namespace PantheonCore::Serialization
{
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
    bool toJson(const T& value, rapidjson::Writer<rapidjson::StringBuffer>& writer) requires std::is_enum_v<T>
    {
        return CHECK(writer.Uint64(static_cast<uint64_t>(value)), "Failed to write \"%s\"", typeid(T).name());
    }

    template <typename T>
    bool fromJson(T& out, const rapidjson::Value& json) requires std::is_enum_v<T>
    {
        if (!CHECK(json.Is<Utility::SmallestUIntT<T>>(), "Failed to read \"%s\"", typeid(T).name()))
            return false;

        out = static_cast<T>(json.Get<Utility::SmallestUIntT<T>>());
        return true;
    }
}
