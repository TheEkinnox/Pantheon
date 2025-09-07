#pragma once
#include "PantheonCore/Utility/EnumFlags.h"

namespace PantheonCore::Utility
{
    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::EnumFlags(EnumT value)
        : m_flags(static_cast<DataT>(value))
    {
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::EnumFlags(DataT value)
        : m_flags(value)
    {
    }

    template <typename EnumT, typename DataT>
    constexpr bool EnumFlags<EnumT, DataT>::operator==(EnumT value) const
    {
        return m_flags == static_cast<DataT>(value);
    }

    template <typename EnumT, typename DataT>
    constexpr bool EnumFlags<EnumT, DataT>::operator==(const EnumFlags& other) const
    {
        return m_flags == other.m_flags;
    }

    template <typename EnumT, typename DataT>
    constexpr bool EnumFlags<EnumT, DataT>::operator!=(EnumT value) const
    {
        return m_flags != static_cast<DataT>(value);
    }

    template <typename EnumT, typename DataT>
    constexpr bool EnumFlags<EnumT, DataT>::operator!=(const EnumFlags& other) const
    {
        return m_flags != other.m_flags;
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::operator&=(EnumT value)
    {
        m_flags &= static_cast<DataT>(value);
        return *this;
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::operator&=(const EnumFlags& other)
    {
        m_flags &= other.m_flags;
        return *this;
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator&(EnumT value) const
    {
        return EnumFlags(m_flags & static_cast<DataT>(value));
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator&(EnumFlags other) const
    {
        return other &= m_flags;
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::operator|=(EnumT value)
    {
        m_flags |= static_cast<DataT>(value);
        return *this;
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::operator|=(const EnumFlags& other)
    {
        m_flags |= other.m_flags;
        return *this;
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator|(EnumT value) const
    {
        return EnumFlags(m_flags | static_cast<DataT>(value));
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator|(EnumFlags other) const
    {
        return other |= m_flags;
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::operator^=(EnumT value)
    {
        m_flags ^= static_cast<DataT>(value);
        return *this;
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::operator^=(const EnumFlags& other)
    {
        m_flags ^= other.m_flags;
        return *this;
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator^(EnumT value) const
    {
        return EnumFlags(m_flags ^ static_cast<DataT>(value));
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator^(EnumFlags other) const
    {
        return other ^= m_flags;
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT> EnumFlags<EnumT, DataT>::operator~() const
    {
        return EnumFlags(~m_flags);
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::operator bool() const
    {
        return m_flags;
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::operator uint8_t() const
    {
        return static_cast<uint8_t>(m_flags);
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::operator uint16_t() const
    {
        return static_cast<uint16_t>(m_flags);
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::operator uint32_t() const
    {
        return static_cast<uint32_t>(m_flags);
    }

    template <typename EnumT, typename DataT>
    constexpr EnumFlags<EnumT, DataT>::operator uint64_t() const
    {
        return static_cast<uint64_t>(m_flags);
    }

    template <typename EnumT, typename DataT>
    EnumFlags<EnumT, DataT>& EnumFlags<EnumT, DataT>::Set(EnumT value, bool state)
    {
        return state ? ((*this) |= value) : ((*this) &= ~value);
    }

    template <typename EnumT, typename DataT>
    constexpr bool EnumFlags<EnumT, DataT>::IsSet(EnumT value) const
    {
        return (m_flags & static_cast<DataT>(value)) == static_cast<DataT>(value);
    }
}
