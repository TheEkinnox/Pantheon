#pragma once

#include "PantheonCore/Utility/utility.h"

#include <algorithm>
#include <cassert>

namespace PantheonCore::Utility
{
    template <typename... Args>
    std::string formatString(const std::string& format, Args&&... args)
    {
        // If no template parameters are passed
        // return the format string as is to avoid unnecessary allocation
        if constexpr (sizeof...(Args) == 0)
        {
            return format;
        }
        else
        {
            // get the formatted text's size
            const int bufferSize = std::snprintf(nullptr, 0, format.c_str(), std::forward<Args>(args)...) + 1;

            assert(bufferSize > 0 && "Unable to format string.");

            // Create a buffer of the computed size
            std::string message;
            message.resize(bufferSize, 0);

            // Write the formatted string in the buffer
            message.resize(std::snprintf(message.data(), bufferSize, format.c_str(), std::forward<Args>(args)...));

            return message;
        }
    }

    template <class CompareFunc>
    void trimStringStart(std::string& str, CompareFunc compareFunc)
    {
        str.erase(str.begin(), std::ranges::find_if_not(str, compareFunc));
    }

    template <class CompareFunc>
    void trimStringEnd(std::string& str, CompareFunc compareFunc)
    {
        str.erase(std::find_if_not(str.rbegin(), str.rend(), compareFunc).base(), str.end());
    }

    template <class CompareFunc>
    void trimString(std::string& str, CompareFunc compareFunc)
    {
        trimStringEnd(str, compareFunc);
        trimStringStart(str, compareFunc);
    }

    constexpr uint64_t readBits(const uint64_t data, const int bitCount, const int offset)
    {
        const uint64_t mask = (1LLU << bitCount) - 1;
        return data >> offset & mask;
    }
}
