#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported frame buffer attachment modes
     */
    enum class EFrameBufferAttachment : uint8_t
    {
        DEPTH,
        STENCIL,
        DEPTH_STENCIL,
        COLOR
    };
}
