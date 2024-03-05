#pragma once
#include <cstdint>

namespace PantheonRendering::RHI
{
    /**
     * \brief Supported rendering apis
     */
    enum class EGraphicsAPI : uint8_t
    {
        NONE,
        OPENGL
    };
}
