#pragma once
#include "PantheonRendering/RHI/IVertexArray.h"

namespace PantheonRendering::RHI
{
    class OpenGLVertexArray final : public IVertexArray
    {
    public:
        /**
         * \brief Creates a vertex array with the given vertex and index buffers
         * \param vbo The linked vertex buffer
         * \param ebo The linked index buffer
         */
        explicit OpenGLVertexArray(const IVertexBuffer& vbo, const IIndexBuffer& ebo);

        /**
         * \brief Disable vertex array copying
         */
        OpenGLVertexArray(const OpenGLVertexArray&) = delete;

        /**
         * \brief Creates a move copy of the given vertex array
         * \param other The vertex array to move
         */
        OpenGLVertexArray(OpenGLVertexArray&& other) noexcept;

        /**
         * \brief Destroys the vertex array
         */
        ~OpenGLVertexArray() override;

        /**
         * \brief Disable vertex array copying
         */
        OpenGLVertexArray& operator=(const OpenGLVertexArray&) = delete;

        /**
         * \brief Moves the given vertex array into this one
         * \param other The vertex array to move
         * \return A reference to the modified vertex array
         */
        OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept;

        /**
         * \brief Binds the vertex attributes object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the vertex attributes object from the current context
         */
        void unbind() const override;

    private:
        uint32_t m_id = 0;
    };
}
