#pragma once
#include "PantheonRendering/RHI/IVertexBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLVertexBuffer final : public IVertexBuffer
    {
    public:
        /**
         * \brief Creates a vertex buffer with the given vertices
         * \param vertices The array of vertices to add to the buffer
         * \param count The number of vertices in the buffer
         */
        OpenGLVertexBuffer(const Geometry::Vertex* vertices, uint32_t count);

        /**
         * \brief Disable vertex buffer copying
         */
        OpenGLVertexBuffer(const OpenGLVertexBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given vertex buffer
         * \param other The vertex buffer to move
         */
        OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept;

        /**
         * \brief Destroys the vertex buffer
         */
        ~OpenGLVertexBuffer() override;

        /**
         * \brief Disable vertex buffer copying
         */
        OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer&) = delete;

        /**
         * \brief Moves the given vertex buffer into this one
         * \param other The vertex buffer to move
         * \return A reference to the modified vertex buffer
         */
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
