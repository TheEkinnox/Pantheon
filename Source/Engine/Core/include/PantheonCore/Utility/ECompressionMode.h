#pragma once

namespace PantheonCore::Utility
{
    enum class ECompressionMode : unsigned char
    {
        NONE,
        ZSTD,
        BROTLI,
        LZ4
    };
}
