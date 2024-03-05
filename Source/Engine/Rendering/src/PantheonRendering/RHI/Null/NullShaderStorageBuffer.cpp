#include "PantheonRendering/RHI/Null/NullShaderStorageBuffer.h"

namespace PantheonRendering::RHI
{
    NullShaderStorageBuffer::NullShaderStorageBuffer(const Enums::EAccessMode accessMode, const uint32_t bindIndex)
        : IShaderStorageBuffer(accessMode, bindIndex)
    {
    }

    void NullShaderStorageBuffer::bind()
    {
    }

    void NullShaderStorageBuffer::unbind()
    {
    }

    void NullShaderStorageBuffer::setRawData(const void*, size_t)
    {
    }

    void NullShaderStorageBuffer::setRawSubData(const void*, size_t, ptrdiff_t)
    {
    }
}
