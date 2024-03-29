#pragma once
#include <cstdint>

namespace PantheonRendering::Core
{
    using LayerMask = uint32_t;

    namespace Layer
    {
        constexpr LayerMask NONE = 0;
        constexpr LayerMask ALL  = 0xffffffff;
    };
}
