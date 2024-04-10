#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported frame buffer target modes
     */
    enum class EFrameBufferTarget : uint32_t
    {
        NONE = 0,
        FRONT_LEFT,
        FRONT_RIGHT,
        BACK_LEFT,
        BACK_RIGHT,
        LEFT,
        RIGHT,
        FRONT,
        BACK,
        COLOR
    };

    /**
     * \brief Gets the color frame buffer target for the given index
     * \param index The color target index
     * \return The resulting color buffer target
     */
    constexpr EFrameBufferTarget makeColorTarget(const uint8_t index)
    {
        return static_cast<EFrameBufferTarget>(static_cast<uint8_t>(EFrameBufferTarget::COLOR) + index);
    }
}
