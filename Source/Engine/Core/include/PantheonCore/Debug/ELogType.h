#pragma once
#include <cstdint>

namespace PantheonCore::Debug
{
    /**
     * \brief Supported log message types
     */
    enum class ELogType : uint8_t
    {
        LOG_RAW     = 0,
        LOG_INFO    = 1 << 0,
        LOG_WARNING = 1 << 1,
        LOG_ERROR   = 1 << 2
    };
}
