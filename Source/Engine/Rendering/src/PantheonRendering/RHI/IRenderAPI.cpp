#include "PantheonRendering/RHI/IRenderAPI.h"

#include "PantheonRendering/RHI/Null/NullAPI.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLAPI.h"

#include <PantheonCore/Debug/Assertion.h>

namespace PantheonRendering::RHI
{
    std::unique_ptr<IRenderAPI> IRenderAPI::s_instance{};

    IRenderAPI& IRenderAPI::setCurrent(const EGraphicsAPI backend)
    {
        if (s_instance && backend == s_instance->getBackend())
            return *s_instance;

        switch (backend)
        {
        case EGraphicsAPI::NONE:
            s_instance = std::make_unique<NullAPI>();
            break;
        case EGraphicsAPI::OPENGL:
            s_instance = std::make_unique<OpenGLAPI>();
            break;
        default:
            ASSERT(false, "Failed to create render API - Unsupported backend");
            break;
        }

        return *s_instance;
    }
}
