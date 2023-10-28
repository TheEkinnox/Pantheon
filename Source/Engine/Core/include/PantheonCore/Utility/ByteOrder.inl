#pragma once
#include "PantheonCore/Utility/ByteOrder.h"

#if __cpp_lib_endian
#include <bit>
#endif

namespace PantheonEngine::Core::Utility
{
    constexpr uint16_t byteSwap16(const uint16_t value)
    {
        return static_cast<uint16_t>((value << 8) | (value >> 8));
    }

    constexpr uint32_t byteSwap32(const uint32_t value)
    {
        return (value << 24)
            | ((value << 8) & 0x00'FF'00'00)
            | ((value >> 8) & 0x00'00'FF'00)
            | (value >> 24);
    }

    constexpr uint64_t byteSwap64(const uint64_t value)
    {
        return (value << 56)
            | ((value << 40) & 0x00'FF'00'00'00'00'00'00)
            | ((value << 24) & 0x00'00'FF'00'00'00'00'00)
            | ((value << 8) & 0x00'00'00'FF'00'00'00'00)
            | ((value >> 8) & 0x00'00'00'00'FF'00'00'00)
            | ((value >> 24) & 0x00'00'00'00'00'FF'00'00)
            | ((value >> 40) & 0x00'00'00'00'00'00'FF'00)
            | (value >> 56);
    }

    template <typename T>
    constexpr T byteSwap(const T value)
    {
        if constexpr (sizeof(T) == sizeof(uint8_t))
        {
            return value;
        }
        else if constexpr (sizeof(T) == sizeof(uint16_t))
        {
            return static_cast<T>(byteSwap16(static_cast<uint16_t>(value)));
        }
        else if constexpr (sizeof(T) == sizeof(uint32_t))
        {
            return static_cast<T>(byteSwap32(static_cast<unsigned long>(value)));
        }
        else if constexpr (sizeof(T) == sizeof(uint64_t))
        {
            return static_cast<T>(byteSwap64(static_cast<uint64_t>(value)));
        }
        else
        {
            T              result;
            const uint8_t* toConvert = reinterpret_cast<const uint8_t*>(&value);
            uint8_t*       converted = reinterpret_cast<uint8_t*>(&result);

            for (size_t i = 0, j = sizeof(T) - 1; i < sizeof(T); ++i, --j)
                converted[i] = toConvert[j];

            return result;
        }
    }

    template <typename T>
    T toBigEndian(T value)
    {
        return isBigEndian() ? value : byteSwap(value);
    }

    template <typename T>
    T fromBigEndian(T value)
    {
        return isBigEndian() ? value : byteSwap(value);
    }

    inline bool isBigEndian()
    {
#ifdef __cpp_lib_endian
        return std::endian::native == std::endian::big;
#else
        static union
        {
            uint32_t integer;
            uint8_t bytes[4];
        } tmp = { 0x01020304 };

        return tmp.bytes[0] == 1;
#endif
    }
}
