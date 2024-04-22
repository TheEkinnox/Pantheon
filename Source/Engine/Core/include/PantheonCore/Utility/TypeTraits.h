#pragma once
#include <type_traits>

namespace PantheonCore::Utility
{
    template <typename T, typename First, typename... Remainder>
    constexpr bool IsOneOf = std::is_same_v<T, First> || IsOneOf<T, Remainder...>;

    template <typename T, typename Other>
    constexpr bool IsOneOf<T, Other> = std::is_same_v<T, Other>;

    template <typename T, typename... Remainder>
    constexpr bool HasDuplicates = IsOneOf<T, Remainder...> || HasDuplicates<Remainder...>;

    template <typename T>
    constexpr bool HasDuplicates<T> = false;

    template <typename T, typename... Remainder>
    constexpr bool IsAllConst = std::is_const_v<T> && IsAllConst<Remainder...>;

    template <typename T>
    constexpr bool IsAllConst<T> = std::is_const_v<T>;

    template <typename T, typename First, typename... Remainder>
    constexpr size_t IndexOf = std::is_same_v<T, First> ? 0 : IndexOf<T, Remainder...> + 1;

    template <typename T, typename First>
    constexpr size_t IndexOf<T, First> = std::is_same_v<T, First> ? 0 : 1;

    template <typename T>
    using SmallestIntT =
    std::conditional_t<sizeof(T) <= sizeof(int8_t), int8_t,
        std::conditional_t<sizeof(T) <= sizeof(int16_t), int16_t,
            std::conditional_t<sizeof(T) <= sizeof(int32_t), int32_t, int64_t>>>;

    template <typename T>
    using SmallestUIntT =
    std::conditional_t<sizeof(T) <= sizeof(uint8_t), uint8_t,
        std::conditional_t<sizeof(T) <= sizeof(uint16_t), uint16_t,
            std::conditional_t<sizeof(T) <= sizeof(uint32_t), uint32_t, uint64_t>>>;
}
