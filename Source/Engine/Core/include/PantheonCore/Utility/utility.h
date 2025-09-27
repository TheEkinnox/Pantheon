#pragma once

#include "TypeTraits.h"

#include <cstdint>
#include <span>
#include <string>
#include <vector>

namespace PantheonCore::Utility
{
    /**
     * \brief Formats a string using the printf syntax
     * \tparam Args The arguments to insert into the format string
     * \param format The format string (follows printf syntax)
     * \param args The arguments to insert into the format string
     * \return The formatted string
     */
    template <class... Args>
    std::string formatString(const char* format, Args&&... args);

    /**
     * \brief Splits the given string using the given delimiter
     * \param str The string to split
     * \param delimiter The delimiter to use to split the string
     * \param includeEmpty Whether empty substrings should be included in the result
     * \return A vector containing the sub-strings
     */
    std::vector<std::string> splitString(const std::string& str, const char* delimiter, bool includeEmpty);

    /**
     * \brief Trims the beginning of the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    template <class CompareFunc = decltype(&isspace)>
    void trimStringStart(std::string& str, CompareFunc compareFunc = &isspace);

    /**
     * \brief Trims the end of the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    template <class CompareFunc = decltype(&isspace)>
    void trimStringEnd(std::string& str, CompareFunc compareFunc = &isspace);

    /**
     * \brief Trims the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    template <class CompareFunc = decltype(&isspace)>
    void trimString(std::string& str, CompareFunc compareFunc = &isspace);

    /**
     * \brief Converts a string to upper case
     * \param str The string to convert to upper case
     */
    void toUpperInPlace(std::string& str);

    /**
     * \brief Converts a string to lower case
     * \param str The string to convert to lower case
     */
    void toLowerInPlace(std::string& str);

    /**
     * \brief Converts a string to upper case
     * \param str The string to convert to upper case
     * \return The string converted to upper case
     */
    std::string toUpper(std::string str);

    /**
     * \brief Converts a string to lower case
     * \param str The string to convert to lower case
     * \return The string converted to lower case
     */
    std::string toLower(std::string str);

    /**
     * \brief Replaces the given substring by another one in the given string
     * \param str The source string
     * \param from The substring to replace
     * \param to The substring to replace by
     */
    void replaceInPlace(std::string& str, const std::string& from, const std::string& to);

    /**
     * \brief Replaces the given substring by another one in the given string
     * \param str The source string
     * \param from The substring to replace
     * \param to The substring to replace by
     */
    std::string replace(std::string str, const std::string& from, const std::string& to);

    /**
     * \brief Extracts the indicated number of bits from the given packed data
     * \param data The packed data
     * \param bitCount The number of bits to read
     * \param offset The offset of the bits to read
     * \return The extracted bits
     */
    constexpr uint64_t readBits(uint64_t data, int bitCount, int offset);

    /**
     * \brief Converts the given value to its underlying type
     * \param value The value to convert
     * \return The value converted to its underlying type
     */
    template <typename T>
    constexpr UnderlyingT<T> underlying(T value);

    /**
     * \brief Creates a string representation of the given size
     * \param size The size to convert to a string
     * \param addInitialValue Whether the initial size should be added to the string
     * \return A string representation of the given size
     */
    std::string sizeToStr(double size, bool addInitialValue = false);

    /**
     * \brief Checks whether the contents of two buffers are identical
     * \param buffer1 The first buffer to compare
     * \param buffer2 The second buffer to compare
     * \return True if the buffers contain the same data, false otherwise
     */
    bool compareBuffers(std::span<const char> buffer1, std::span<const char> buffer2);
}

#include "PantheonCore/Utility/utility.inl"
