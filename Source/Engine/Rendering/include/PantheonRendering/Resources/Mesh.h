#pragma once
#include "PantheonRendering/Geometry/Vertex.h"
#include "PantheonRendering/Enums/EPrimitiveType.h"
#include "PantheonRendering/Geometry/BoundingBox.h"
#include "PantheonRendering/RHI/IVertexArray.h"

#include <PantheonCore/Serialization/IByteSerializable.h>

namespace PantheonRendering::Resources
{
    class Mesh final : public PantheonCore::Serialization::IByteSerializable
    {
    public:
        /**
         * \brief Creates an empty mesh
         */
        Mesh() = default;

        /**
         * \brief Creates a mesh with the given vertices, indices, primitive type and material index
         * \param vertices The mesh's vertices
         * \param indices The mesh's indices
         * \param primitiveType The mesh's primitive type
         * \param materialIndex The mesh's material index
         */
        Mesh(std::vector<Geometry::Vertex> vertices, std::vector<uint32_t> indices,
             Enums::EPrimitiveType         primitiveType, uint32_t         materialIndex);

        /**
         * \brief Creates a copy of the given mesh
         * \param other The mesh to copy
         */
        Mesh(const Mesh& other);

        /**
         * \brief Creates a move copy of the given mesh
         * \param other The mesh to move
         */
        Mesh(Mesh&& other) noexcept = default;

        /**
         * \brief Destroys the mesh
         */
        ~Mesh() override = default;

        /**
         * \brief Assigns a copy of the given mesh to this one
         * \param other The mesh to copy
         * \return A reference to the modified mesh
         */
        Mesh& operator=(const Mesh& other);

        /**
         * \brief Moves the given mesh into this one
         * \param other The moved mesh
         * \return A reference to the modified mesh
         */
        Mesh& operator=(Mesh&& other) noexcept = default;

        /**
         * \brief Serializes the mesh to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the mesh from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

        /**
         * \brief Initializes the mesh
         * \return True if the mesh was successfully initialized. False otherwise.
         */
        bool init();

        /**
         * \brief Binds the mesh's buffers
         */
        void bind() const;

        /**
         * \brief Unbinds the mesh's buffers
         */
        void unbind() const;

        /**
         * \brief Gets the mesh's element count
         * \return The mesh's number of indices
         */
        uint32_t getIndexCount() const;

        /**
         * \brief Gets the mesh's bounding box
         * \return The mesh's bounding box
         */
        Geometry::BoundingBox getBoundingBox() const;

        /**
         * \brief Gets the mesh's primitive type
         * \return The mesh's primitive type
         */
        Enums::EPrimitiveType getPrimitiveType() const;

        /**
         * \brief Gets the mesh's material index
         * \return The mesh's material index
         */
        uint32_t getMaterialIndex() const;

    private:
        std::vector<Geometry::Vertex> m_vertices;
        std::vector<uint32_t>         m_indices;

        Geometry::BoundingBox m_boundingBox;
        Enums::EPrimitiveType m_primitiveType;
        uint32_t              m_materialIndex;

        std::unique_ptr<RHI::IVertexBuffer> m_vbo;
        std::unique_ptr<RHI::IIndexBuffer>  m_ebo;
        std::unique_ptr<RHI::IVertexArray>  m_vao;

        /**
         * \brief Converts the vertex' members to big endian
         * \param vertex The vertex to convert to big endian
         */
        static void vertexToBigEndian(Geometry::Vertex& vertex);

        /**
         * \brief Converts the vertex' members from big endian to the system's byte order
         * \param vertex The vertex to convert from big endian
         */
        static void vertexFromBigEndian(Geometry::Vertex& vertex);
    };
}
