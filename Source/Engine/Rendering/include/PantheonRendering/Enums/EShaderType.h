#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported shader types
     */
    enum class EShaderType : uint8_t
    {
        UNKNOWN,
        VERTEX,
        FRAGMENT,
        GEOMETRY,
        TESSELATION_EVALUATION,
        TESSELATION_CONTROL,
        COMPUTE
    };
}
