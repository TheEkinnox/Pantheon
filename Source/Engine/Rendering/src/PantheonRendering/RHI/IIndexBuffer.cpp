#include "PantheonRendering/RHI/IIndexBuffer.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullIndexBuffer.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLIndexBuffer.h"

#include <PantheonCore/Utility/ServiceLocator.h>

namespace PantheonRendering::RHI
{
    std::unique_ptr<IIndexBuffer> IIndexBuffer::create(const uint32_t* indices, uint32_t count)
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullIndexBuffer>(indices, count);
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLIndexBuffer>(indices, count);
        default:
            ASSERT(false, "Failed to create index buffer - Unsupported graphics api");
            return {};
        }
    }

    std::unique_ptr<IIndexBuffer> IIndexBuffer::create(const std::vector<uint32_t>& indices)
    {
        return create(indices.data(), static_cast<uint32_t>(indices.size()));
    }
}
