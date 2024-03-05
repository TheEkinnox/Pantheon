#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported depth/stencil operations compare algorithms
     */
    enum class ECompareAlgorithm : uint8_t
    {
        NEVER,
        LESS,
        EQUAL,
        LESS_EQUAL,
        GREATER,
        NOT_EQUAL,
        GREATER_EQUAL,
        ALWAYS
    };
}
