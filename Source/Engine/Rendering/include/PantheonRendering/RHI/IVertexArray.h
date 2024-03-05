#pragma once
#include "PantheonRendering/RHI/IIndexBuffer.h"
#include "PantheonRendering/RHI/IVertexBuffer.h"

namespace PantheonRendering::RHI
{
    class IVertexArray
    {
    public:
        IVertexArray(const IVertexArray&)           = delete;
        IVertexArray(IVertexArray&& other) noexcept = default;
        virtual ~IVertexArray()                     = default;

        IVertexArray& operator=(const IVertexArray&)           = delete;
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
        IVertexArray() = default;
    };
}
