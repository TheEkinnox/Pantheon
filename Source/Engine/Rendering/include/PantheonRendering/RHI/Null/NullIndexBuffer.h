#pragma once
#include "PantheonRendering/RHI/IIndexBuffer.h"

namespace PantheonRendering::RHI
{
    class NullIndexBuffer final : public IIndexBuffer
    {
    public:
        NullIndexBuffer(const uint32_t* indices, uint32_t count);
        NullIndexBuffer(const NullIndexBuffer&)     = delete;
        NullIndexBuffer(NullIndexBuffer&& other) noexcept = default;

        ~NullIndexBuffer() override = default;

        NullIndexBuffer& operator=(const NullIndexBuffer&)     = delete;
        NullIndexBuffer& operator=(NullIndexBuffer&& other) noexcept = default;

        /**
         * \brief Binds the index buffer object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the index buffer from the current context
         */
        void unbind() const override;
    };
}
