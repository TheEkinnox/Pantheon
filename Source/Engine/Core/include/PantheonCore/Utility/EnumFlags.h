#pragma once
#include <cstdint>

namespace PantheonCore::Utility
{
    template <typename EnumT, typename DataT = std::underlying_type_t<EnumT>>
    class EnumFlags
    {
    public:
        using DataType = DataT;

        constexpr EnumFlags() = default;
        constexpr EnumFlags(EnumT value);
        constexpr EnumFlags(DataT value);

        EnumFlags(const EnumFlags& other)     = default;
        EnumFlags(EnumFlags&& other) noexcept = default;
        ~EnumFlags()                          = default;

        EnumFlags& operator=(const EnumFlags& other)     = default;
        EnumFlags& operator=(EnumFlags&& other) noexcept = default;

        constexpr bool operator==(EnumT value) const;
        constexpr bool operator==(const EnumFlags& other) const;
        constexpr bool operator!=(EnumT value) const;
        constexpr bool operator!=(const EnumFlags& other) const;

        EnumFlags&          operator&=(EnumT value);
        EnumFlags&          operator&=(const EnumFlags& other);
        constexpr EnumFlags operator&(EnumT value) const;
        constexpr EnumFlags operator&(EnumFlags other) const;

        EnumFlags&          operator|=(EnumT value);
        EnumFlags&          operator|=(const EnumFlags& other);
        constexpr EnumFlags operator|(EnumT value) const;
        constexpr EnumFlags operator|(EnumFlags other) const;

        EnumFlags&          operator^=(EnumT value);
        EnumFlags&          operator^=(const EnumFlags& other);
        constexpr EnumFlags operator^(EnumT value) const;
        constexpr EnumFlags operator^(EnumFlags other) const;

        constexpr EnumFlags operator~() const;

        constexpr operator bool() const;
        constexpr operator uint8_t() const;
        constexpr operator uint16_t() const;
        constexpr operator uint32_t() const;
        constexpr operator uint64_t() const;

        EnumFlags&     Set(EnumT value, bool state);
        constexpr bool IsSet(EnumT value) const;

        constexpr friend EnumFlags operator&(const EnumT a, const EnumFlags& b)
        {
            return EnumFlags(a & b.m_flags);
        }

        constexpr friend EnumFlags operator|(const EnumT a, const EnumFlags& b)
        {
            return EnumFlags(a | b.m_flags);
        }

        constexpr friend EnumFlags operator^(const EnumT a, const EnumFlags& b)
        {
            return EnumFlags(a ^ b.m_flags);
        }

    private:
        DataT m_flags = 0;
    };
}

#define PTH_ENUM_FLAGS(EnumClass)                                     \
using EnumClass##Flags = PantheonCore::Utility::EnumFlags<EnumClass>; \
                                                                      \
constexpr EnumClass##Flags operator&(EnumClass Left, EnumClass Right) \
{                                                                     \
    return EnumClass##Flags(Left) & Right;                            \
}                                                                     \
                                                                      \
constexpr EnumClass##Flags operator|(EnumClass Left, EnumClass Right) \
{                                                                     \
    return EnumClass##Flags(Left) | Right;                            \
}                                                                     \
                                                                      \
constexpr EnumClass##Flags operator^(EnumClass Left, EnumClass Right) \
{                                                                     \
    return EnumClass##Flags(Left) ^ Right;                            \
}                                                                     \
                                                                      \
constexpr EnumClass##Flags operator~(const EnumClass Val)             \
{                                                                     \
    return ~EnumClass##Flags(Val);                                    \
}

#include "PantheonCore/Utility/EnumFlags.inl"
