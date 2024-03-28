#pragma once
#include "PantheonRendering/RHI/IShaderStorageBuffer.h"

namespace PantheonRendering::RHI
{
    template <typename T>
    void IShaderStorageBuffer::setData(const T* data, const size_t count)
    {
        setRawData(static_cast<const void*>(data), count * sizeof(T));
    }

    template <typename T>
    void IShaderStorageBuffer::setSubData(const T* data, const size_t count, const ptrdiff_t offset)
    {
        setRawSubData(static_cast<const void*>(data), count * sizeof(T), offset);
    }
}
