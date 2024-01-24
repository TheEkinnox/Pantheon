#pragma once

#include <functional>
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
    std::string formatString(const std::string& format, Args&&... args);

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
    inline void trimStringStart(std::string& str, const std::function<bool(char)>& compareFunc = &isspace);

    /**
     * \brief Trims the end of the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    inline void trimStringEnd(std::string& str, const std::function<bool(char)>& compareFunc = &isspace);

    /**
     * \brief Trims the given string using the given compare func
     * \param str The string to trim
     * \param compareFunc The function used to check if a character should be trimmed
     */
    inline void trimString(std::string& str, const std::function<bool(char)>& compareFunc = &isspace);

    /**
     * \brief Extracts the indicated number of bits from the given packed data
     * \param data The packed data
     * \param bitCount The number of bits to read
     * \param offset The offset of the bits to read
     * \return The extracted bits
     */
    constexpr uint64_t readBits(uint64_t data, int bitCount, int offset);

    /**
     * \brief Creates a string representation of the given size
     * \param size The size to convert to a string
     * \param addInitialValue Whether the initial size should be added to the string
     * \return A string representation of the given size
     */
    std::string sizeToStr(double size, bool addInitialValue = false);
}

#include "PantheonCore/Utility/utility.inl"
