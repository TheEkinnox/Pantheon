#pragma once
#include "PantheonRendering/RHI/IVertexBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLVertexBuffer final : public IVertexBuffer
    {
    public:
        OpenGLVertexBuffer(const Geometry::Vertex* vertices, uint32_t count);

        OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;
        OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;

        ~OpenGLVertexBuffer() override;

        OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;
        OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& other) noexcept;

        /**
         * \brief Binds the vertex buffer to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the vertex buffer from the current context
         */
        void unbind() const override;

    private:
        uint32_t m_id = 0;
    };
}
