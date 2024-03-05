#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported light types
     */
    enum class ELightType : uint8_t
    {
        AMBIENT,
        DIRECTIONAL,
        POINT,
        SPOT
    };
}
