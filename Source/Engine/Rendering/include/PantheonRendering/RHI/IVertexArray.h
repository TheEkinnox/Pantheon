#pragma once
#include "PantheonRendering/RHI/IIndexBuffer.h"
#include "PantheonRendering/RHI/IVertexBuffer.h"

namespace PantheonRendering::RHI
{
    class IVertexArray
    {
    public:
        /**
         * \brief Disable vertex array copying
         */
        IVertexArray(const IVertexArray&) = delete;

        /**
         * \brief Creates a move copy of the given vertex array
         * \param other The vertex array to move
         */
        IVertexArray(IVertexArray&& other) noexcept = default;

        /**
         * \brief Destroys the vertex array
         */
        virtual ~IVertexArray() = default;

        /**
         * \brief Disable vertex array copying
         */
        IVertexArray& operator=(const IVertexArray&) = delete;

        /**
         * \brief Moves the given vertex array into this one
         * \param other The vertex array to move
         * \return A reference to the modified vertex array
         */
        IVertexArray& operator=(IVertexArray&& other) noexcept = default;

        /**
         * \brief Binds the vertex attributes object to the current context
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds the vertex attributes object from the current context
         */
        virtual void unbind() const = 0;

        /**
         * \brief Creates a vertex array for the current render API with the given vertex and index buffers
         * \param vbo The vertex array's vertex buffer
         * \param ebo The vertex array's index buffer
         * \return The created vertex array
         */
        static std::unique_ptr<IVertexArray> create(const IVertexBuffer& vbo, const IIndexBuffer& ebo);

    protected:
        /**
         * \brief Creates a default vertex array
         */
        IVertexArray() = default;
    };
}
