#include "PantheonRendering/RHI/IVertexBuffer.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullVertexBuffer.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLVertexBuffer.h"

using namespace PantheonRendering::Geometry;

namespace PantheonRendering::RHI
{
    std::unique_ptr<IVertexBuffer> IVertexBuffer::create(const Vertex* vertices, const uint32_t count)
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullVertexBuffer>(vertices, count);
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLVertexBuffer>(vertices, count);
        default:
            ASSERT(false, "Failed to create vertex buffer - Unsupported graphics api");
            return {};
        }
    }

    std::unique_ptr<IVertexBuffer> IVertexBuffer::create(const std::vector<Vertex>& vertices)
    {
        return create(vertices.data(), static_cast<uint32_t>(vertices.size()));
    }
}
