#include "PantheonRendering/RHI/IVertexArray.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullVertexArray.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLVertexArray.h"

#include <PantheonCore/Utility/ServiceLocator.h>

namespace PantheonRendering::RHI
{
    std::unique_ptr<IVertexArray> IVertexArray::create(const IVertexBuffer& vbo, const IIndexBuffer& ebo)
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullVertexArray>(vbo, ebo);
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLVertexArray>(vbo, ebo);
        default:
            ASSERT(false, "Failed to create vertex array - Unsupported graphics api");
            return {};
        }
    }
}
