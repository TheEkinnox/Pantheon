#pragma once
#include <cstdint>

namespace PantheonEngine::Core::Utility
{
    /**
     * \brief Checks whether the current system uses a big endian architecture
     * \return True if the current system uses a big endian architecture. False otherwise
     */
    bool isBigEndian();

    /**
     * \brief Reverses the given value's byte order
     * \param value The value to convert
     * \return The converted value
     */
    constexpr uint16_t byteSwap16(uint16_t value);

    /**
     * \brief Reverses the given value's byte order
     * \param value The value to convert
     * \return The converted value
     */
    constexpr uint32_t byteSwap32(uint32_t value);

    /**
     * \brief Reverses the given value's byte order
     * \param value The value to convert
     * \return The converted value
     */
    constexpr uint64_t byteSwap64(uint64_t value);

    /**
     * \brief Reverses the given value's byte order
     * \tparam T The converted value's type
     * \param value The value to convert
     * \return The converted value
     */
    template <typename T>
    constexpr T byteSwap(T value);

    /**
     * \brief Converts the given value to big endian
     * \tparam T The converted value's type
     * \param value The value to convert
     * \return The converted value
     */
    template <typename T>
    T toBigEndian(T value);

    /**
     * \brief Converts the given value from big endian
     * \tparam T The converted value's type
     * \param value The value to convert
     * \return The converted value
     */
    template <typename T>
    T fromBigEndian(T value);
}

#include "PantheonCore/Utility/ByteOrder.inl"
