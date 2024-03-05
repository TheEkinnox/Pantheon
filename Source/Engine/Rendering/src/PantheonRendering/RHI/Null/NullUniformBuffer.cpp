#include "PantheonRendering/RHI/Null/NullUniformBuffer.h"

namespace PantheonRendering::RHI
{
    NullUniformBuffer::NullUniformBuffer(const Enums::EAccessMode accessSpecifier, const uint32_t bindIndex)
        : IUniformBuffer(accessSpecifier, bindIndex)
    {
    }

    void NullUniformBuffer::bind()
    {
    }

    void NullUniformBuffer::unbind()
    {
    }

    void NullUniformBuffer::setRawData(const void*, size_t)
    {
    }

    void NullUniformBuffer::setRawSubData(const void*, size_t, ptrdiff_t)
    {
    }
}
