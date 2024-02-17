#pragma once

namespace PantheonApp::Input
{
    enum class EInputModifier
    {
        SHIFT     = 1 << 0,
        CONTROL   = 1 << 1,
        ALT       = 1 << 2,
        SUPER     = 1 << 3,
        CAPS_LOCK = 1 << 4,
        NUM_LOCK  = 1 << 5,
        NONE      = 0
    };
}
