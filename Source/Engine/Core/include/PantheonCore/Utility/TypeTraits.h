#pragma once
#include <climits>
#include <cstdint>
#include <cstddef>
#include <type_traits>

namespace PantheonCore::Utility
{
    template <typename T, typename First, typename... Remainder>
    inline constexpr bool IsOneOf = std::is_same_v<T, First> || IsOneOf<T, Remainder...>;

    template <typename T, typename Other>
    inline constexpr bool IsOneOf<T, Other> = std::is_same_v<T, Other>;

    template <typename T, typename... Remainder>
    inline constexpr bool HasDuplicates = IsOneOf<T, Remainder...> || HasDuplicates<Remainder...>;

    template <typename T>
    inline constexpr bool HasDuplicates<T> = false;

    template <typename T, typename... Remainder>
    inline constexpr bool IsAllConst = std::is_const_v<T> && IsAllConst<Remainder...>;

    template <typename T>
    inline constexpr bool IsAllConst<T> = std::is_const_v<T>;

    template <typename T, typename First, typename... Remainder>
    inline constexpr size_t IndexOf = std::is_same_v<T, First> ? 0 : IndexOf<T, Remainder...> + 1;

    template <typename T, typename First>
    inline constexpr size_t IndexOf<T, First> = std::is_same_v<T, First> ? 0 : 1;

    template <uint8_t Size>
    using SmallestInt =
    std::conditional_t<Size <= 8, int8_t,
        std::conditional_t<Size <= 16, int16_t,
            std::conditional_t<Size <= 32, int32_t, int64_t>>>;

    template <uint8_t Size>
    using SmallestUInt =
    std::conditional_t<Size <= 8, uint8_t,
        std::conditional_t<Size <= 16, uint16_t,
            std::conditional_t<Size <= 32, uint32_t, uint64_t>>>;

    template <typename T>
    using SmallestIntT = SmallestInt<sizeof(T) * CHAR_BIT>;

    template <typename T>
    using SmallestUIntT = SmallestUInt<sizeof(T) * CHAR_BIT>;

    namespace Detail
    {
        template <class T, bool = std::is_enum_v<T>>
        struct UnderlyingType
        {
            using type = std::underlying_type_t<T>;
        };

        template <class T>
        struct UnderlyingType<T, false>
        {
            using type = T;
        };
    }

    template <class T>
    using UnderlyingT = Detail::UnderlyingType<T>::type; // Hack to silently fallback to T when not an enum
}
