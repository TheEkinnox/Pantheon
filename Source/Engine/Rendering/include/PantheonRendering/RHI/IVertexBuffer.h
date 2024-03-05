#pragma once
#include "PantheonRendering/Geometry/Vertex.h"

#include <cstdint>
#include <memory>
#include <vector>

namespace PantheonRendering::RHI
{
    class IVertexBuffer
    {
    public:
        /**
         * \brief Disable vertex buffer copying
         */
        IVertexBuffer(const IVertexBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given vertex buffer
         * \param other The vertex buffer to move
         */
        IVertexBuffer(IVertexBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the vertex buffer
         */
        virtual ~IVertexBuffer() = default;

        /**
         * \brief Disable vertex buffer copying
         */
        IVertexBuffer& operator=(const IVertexBuffer&) = delete;

        /**
         * \brief Moves the given vertex buffer into this one
         * \param other The vertex buffer to move
         * \return A reference to the modified vertex buffer
         */
        IVertexBuffer& operator=(IVertexBuffer&& other) noexcept = default;

        /**
         * \brief Binds the vertex buffer to the current context
         */
        virtual void bind() const = 0;

        /**
         * \brief Unbinds the vertex buffer from the current context
         */
        virtual void unbind() const = 0;

        /**
         * \brief Creates a vertex buffer for the current render API with the given vertices
         * \param vertices The array of vertices to add to the buffer
         * \param count The number of vertices in the buffer
         * \return The created vertex buffer
         */
        static std::unique_ptr<IVertexBuffer> create(const Geometry::Vertex* vertices, uint32_t count);

        /**
         * \brief Creates a vertex buffer for the current render API with the given vertices
         * \param vertices The array of vertices to add to the buffer
         * \return The created vertex buffer
         */
        static std::unique_ptr<IVertexBuffer> create(const std::vector<Geometry::Vertex>& vertices);

    protected:
        IVertexBuffer() = default;
    };
}
