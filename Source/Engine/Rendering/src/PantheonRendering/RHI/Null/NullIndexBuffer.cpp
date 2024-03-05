#include "PantheonRendering/RHI/Null/NullIndexBuffer.h"

namespace PantheonRendering::RHI
{
    NullIndexBuffer::NullIndexBuffer(const uint32_t*, uint32_t)
    {
    }

    void NullIndexBuffer::bind() const
    {
    }

    void NullIndexBuffer::unbind() const
    {
    }
}
