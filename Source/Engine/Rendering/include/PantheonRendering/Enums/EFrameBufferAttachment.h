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

    /**
     * \brief Gets the color frame buffer attachment for the given index
     * \param index The color attachment index
     * \return The resulting color buffer attachment
     */
    constexpr EFrameBufferAttachment makeColorAttachment(const uint8_t index)
    {
        return static_cast<EFrameBufferAttachment>(static_cast<uint8_t>(EFrameBufferAttachment::COLOR) + index);
    }
}
