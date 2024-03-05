#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported blend factors
     */
    enum class EBlendFactor : uint8_t
    {
        ZERO,                       // Factor is equal to 0.
        ONE,                        // Factor is equal to 1.
        SRC_COLOR,                  // Factor is equal to the source color vector.
        ONE_MINUS_SRC_COLOR,        // Factor is equal to 1 minus the source color vector.
        SRC_ALPHA,                  // Factor is equal to the alpha component of the source color vector.
        ONE_MINUS_SRC_ALPHA,        // Factor is equal to 1-alpha of the source color vector.
        DST_ALPHA,                  // Factor is equal to the alpha component of the destination color vector.
        ONE_MINUS_DST_ALPHA,        // Factor is equal to 1-alpha of the destination color vector.
        DST_COLOR,                  // Factor is equal to the destination color vector.
        ONE_MINUS_DST_COLOR,        // Factor is equal to 1 minus the destination color vector.
        CONSTANT_COLOR,             // Factor is equal to the constant color vector.
        ONE_MINUS_CONSTANT_COLOR,   // Factor is equal to 1 - the constant color vector.
        CONSTANT_ALPHA,             // Factor is equal to the alpha component of the constant color vector.
        ONE_MINUS_CONSTANT_ALPHA,   // Factor is equal to 1-alpha of the constant color vector.
    };
}
