#include "PantheonRendering/RHI/IFrameBuffer.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullFrameBuffer.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLFrameBuffer.h"

#include <PantheonCore/Debug/Assertion.h>

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
            PTH_ASSERT(false, "Failed to create frame buffer - Unsupported graphics api");
            return {};
        }
    }

    void IFrameBuffer::bindDefault()
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            NullFrameBuffer::bindDefault();
            break;
        case EGraphicsAPI::OPENGL:
            OpenGLFrameBuffer::bindDefault();
            break;
        default:
            PTH_ASSERT(false, "Failed to bind default frame buffer - Unsupported graphics api");
            break;
        }
    }
}
