#include "PantheonRendering/RHI/IFrameBuffer.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLFrameBuffer.h"
#include "PantheonRendering/RHI/Null/NullFrameBuffer.h"

#include <PantheonCore/Utility/ServiceLocator.h>

namespace PantheonRendering::RHI
{
    std::unique_ptr<IFrameBuffer> IFrameBuffer::create()
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullFrameBuffer>();
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLFrameBuffer>();
        default:
            ASSERT(false, "Failed to create frame buffer - Unsupported graphics api");
            return {};
        }
    }
}
