#pragma once
#include "PantheonRendering/RHI/IVertexArray.h"

namespace PantheonRendering::RHI
{
    class NullVertexArray final : public IVertexArray
    {
    public:
        /**
         * \brief Creates a vertex array with the given vertex and index buffers
         * \param vbo The linked vertex buffer
         * \param ebo The linked index buffer
         */
        explicit NullVertexArray(const IVertexBuffer& vbo, const IIndexBuffer& ebo);

        /**
         * \brief Disable vertex array copying
         */
        NullVertexArray(const NullVertexArray&) = delete;

        /**
         * \brief Creates a move copy of the given vertex array
         * \param other The vertex array to move
         */
        NullVertexArray(NullVertexArray&& other) noexcept = default;

        /**
         * \brief Destroys the vertex array
         */
        ~NullVertexArray() override = default;

        /**
         * \brief Disable vertex array copying
         */
        NullVertexArray& operator=(const NullVertexArray&) = delete;

        /**
         * \brief Moves the given vertex array into this one
         * \param other The vertex array to move
         * \return A reference to the modified vertex array
         */
        NullVertexArray& operator=(NullVertexArray&& other) noexcept = default;

        /**
         * \brief Binds the vertex attributes object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the vertex attributes object from the current context
         */
        void unbind() const override;
    };
}
