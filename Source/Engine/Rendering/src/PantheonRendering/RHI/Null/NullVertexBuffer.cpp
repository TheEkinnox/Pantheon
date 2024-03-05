#include "PantheonRendering/RHI/Null/NullVertexBuffer.h"

namespace PantheonRendering::RHI
{
    NullVertexBuffer::NullVertexBuffer(const Geometry::Vertex*, uint32_t)
    {
    }

    void NullVertexBuffer::bind() const
    {
    }

    void NullVertexBuffer::unbind() const
    {
    }
}
