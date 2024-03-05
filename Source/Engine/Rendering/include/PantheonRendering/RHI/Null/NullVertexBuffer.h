#pragma once
#include "PantheonRendering/RHI/IVertexBuffer.h"

namespace PantheonRendering::RHI
{
    class NullVertexBuffer final : public IVertexBuffer
    {
    public:
        /**
         * \brief Creates a vertex buffer with the given vertices
         * \param vertices The array of vertices to add to the buffer
         * \param count The number of vertices in the buffer
         */
        NullVertexBuffer(const Geometry::Vertex* vertices, uint32_t count);

        /**
         * \brief Disable vertex buffer copying
         */
        NullVertexBuffer(const NullVertexBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given vertex buffer
         * \param other The vertex buffer to move
         */
        NullVertexBuffer(NullVertexBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the vertex buffer
         */
        ~NullVertexBuffer() override = default;

        /**
         * \brief Disable vertex buffer copying
         */
        NullVertexBuffer& operator=(const NullVertexBuffer&) = delete;

        /**
         * \brief Moves the given vertex buffer into this one
         * \param other The vertex buffer to move
         * \return A reference to the modified vertex buffer
         */
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
