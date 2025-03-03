#pragma once
#include <cstdint>
#include <PantheonCore/Utility/CoreDefines.h>

namespace PantheonRendering::RHI
{
    /**
     * \brief Supported rendering apis
     */
    enum class EGraphicsAPI : uint8_t
    {
        NONE,
#if USING(PTH_FEATURE_OPENGL)
        OPENGL
#endif
    };
}
