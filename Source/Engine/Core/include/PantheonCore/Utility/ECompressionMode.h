#pragma once

namespace PantheonEngine::Core::Utility
{
    enum class ECompressionMode : unsigned char
    {
        NONE,
        ZSTD,
        BROTLI,
        LZ4
    };
}
