#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported culling modes
     */
    enum class ECullingMode : uint8_t
    {
        NONE,
        MODEL,
        MESH
    };
}
