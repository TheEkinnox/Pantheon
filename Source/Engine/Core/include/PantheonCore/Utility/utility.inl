#pragma once

#include <stdexcept>

#include "PantheonCore/Utility/utility.h"

namespace PantheonEngine::Core::Utility
{
    template <typename... Args>
    std::string formatString(const char* const format, Args&&... args)
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
            const int bufferSize = std::snprintf(nullptr, 0, format, std::forward<Args>(args)...) + 1;

            if (bufferSize <= 0)
                throw std::runtime_error("Unable to format string.");

            // Create a buffer of the computed size
            std::vector<char> buffer;
            buffer.reserve(bufferSize);

            // Write the formatted string in the buffer
            std::snprintf(buffer.data(), bufferSize, format, std::forward<Args>(args)...);

            // Copy the buffer data into an std::string
            std::string message(buffer.data(), buffer.data() + bufferSize - 1);

            return message;
        }
    }

    constexpr uint64_t readBits(const uint64_t data, const int bitCount, const int offset)
    {
        const uint64_t mask = (1LLU << bitCount) - 1;
        return data >> offset & mask;
    }
}
