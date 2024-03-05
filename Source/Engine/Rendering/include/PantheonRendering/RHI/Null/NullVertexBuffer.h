#pragma once
#include "PantheonRendering/RHI/IVertexBuffer.h"

namespace PantheonRendering::RHI
{
    class NullVertexBuffer final : public IVertexBuffer
    {
    public:
        NullVertexBuffer(const Geometry::Vertex* vertices, uint32_t count);

        NullVertexBuffer(const NullVertexBuffer&)           = delete;
        NullVertexBuffer(NullVertexBuffer&& other) noexcept = default;

        ~NullVertexBuffer() override = default;

        NullVertexBuffer& operator=(const NullVertexBuffer&)           = delete;
        NullVertexBuffer& operator=(NullVertexBuffer&& other) noexcept = default;

        /**
         * \brief Binds the vertex buffer to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the vertex buffer from the current context
         */
        void unbind() const override;
    };
}
