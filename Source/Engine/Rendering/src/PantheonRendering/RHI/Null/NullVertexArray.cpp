#include "PantheonRendering/RHI/Null/NullVertexArray.h"

namespace PantheonRendering::RHI
{
    NullVertexArray::NullVertexArray(const IVertexBuffer&, const IIndexBuffer&)
    {
    }

    void NullVertexArray::bind() const
    {
    }

    void NullVertexArray::unbind() const
    {
    }
}
