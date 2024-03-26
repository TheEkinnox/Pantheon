#pragma once
#include <cstdint>

namespace PantheonCore::Utility
{
    enum class ECompressionMode : uint8_t
    {
        NONE,
        ZSTD,
        BROTLI,
        LZ4
    };
}
