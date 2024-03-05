#pragma once
#include "PantheonRendering/RHI/IRenderAPI.h"

#include <PantheonCore/Debug/Assertion.h>

namespace PantheonRendering::RHI
{
    inline IRenderAPI& IRenderAPI::getCurrent()
    {
        ASSERT(s_instance, "No loaded rendering API");
        return *s_instance;
    }
}
