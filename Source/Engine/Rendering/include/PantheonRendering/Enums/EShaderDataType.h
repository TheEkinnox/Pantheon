#pragma once
#include <cstdint>

namespace PantheonRendering::Enums
{
    enum class EShaderDataType : uint8_t
    {
        UNKNOWN,
        BOOL,
        INT,
        UNSIGNED_INT,
        FLOAT,
        VEC2,
        VEC3,
        VEC4,
        MAT3,
        MAT4,
        TEXTURE
    };
}
