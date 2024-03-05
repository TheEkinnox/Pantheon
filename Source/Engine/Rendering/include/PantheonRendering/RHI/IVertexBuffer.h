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
        IVertexBuffer(const IVertexBuffer&)           = delete;
        IVertexBuffer(IVertexBuffer&& other) noexcept = default;

        virtual ~IVertexBuffer() = default;

        IVertexBuffer& operator=(const IVertexBuffer&)           = delete;
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
