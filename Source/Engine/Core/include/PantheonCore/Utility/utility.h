#pragma once

#include <string>
#include <vector>

namespace PantheonEngine::Core::Utility
{
    /**
     * \brief Formats a string using the printf syntax
     * \tparam Args The arguments to insert into the format string
     * \param format The format string (follows printf syntax)
     * \param args The arguments to insert into the format string
     * \return The formatted string
     */
    template <class... Args>
    std::string formatString(const char* format, Args... args);

    /**
     * \brief Splits the given string using the given delimiter
     * \param str The string to split
     * \param delimiter The delimiter to use to split the string
     * \param includeEmpty Whether empty substrings should be included in the result
     * \return A vector containing the sub-strings
     */
    std::vector<std::string> splitString(const std::string& str, const char* delimiter, bool includeEmpty);

    /**
     * \brief Extracts the indicated number of bits from the given packed data
     * \param data The packed data
     * \param bitCount The number of bits to read
     * \param offset The offset of the bits to read
     * \return The extracted bits
     */
    constexpr uint64_t readBits(uint64_t data, int bitCount, int offset);
}

#include "PantheonCore/Utility/utility.inl"
