#include "PantheonRendering/Resources/Mesh.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>

using namespace LibMath;
using namespace PantheonCore::Utility;
using namespace PantheonRendering::RHI;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::Geometry;

namespace PantheonRendering::Resources
{
    Mesh::Mesh(std::vector<Vertex>  vertices, std::vector<uint32_t> indices,
               const EPrimitiveType primitiveType, const uint32_t   materialIndex)
        : m_vertices(std::move(vertices)), m_indices(std::move(indices)),
        m_primitiveType(primitiveType), m_materialIndex(materialIndex)
    {
        m_boundingBox =
        {
            Vector3(std::numeric_limits<float>::max()),
            Vector3(std::numeric_limits<float>::lowest())
        };

        for (const Vertex& vertex : m_vertices)
        {
            m_boundingBox.m_min = min(m_boundingBox.m_min, vertex.m_position);
            m_boundingBox.m_max = max(m_boundingBox.m_max, vertex.m_position);
        }
    }

    Mesh::Mesh(const Mesh& other)
        : Mesh(other.m_vertices, other.m_indices, other.m_primitiveType, other.m_materialIndex)
    {
        init();
    }

    Mesh& Mesh::operator=(const Mesh& other)
    {
        if (this == &other)
            return *this;

        m_vertices      = other.m_vertices;
        m_indices       = other.m_indices;
        m_primitiveType = other.m_primitiveType;
        m_materialIndex = other.m_materialIndex;

        init();

        return *this;
    }

    bool Mesh::toBinary(std::vector<char>& output) const
    {
        if (m_indices.empty() || m_vertices.empty())
            return false;

        const ElemCountT vertCount      = static_cast<ElemCountT>(m_vertices.size());
        const size_t     vertBufferSize = vertCount * sizeof(Vertex);

        const ElemCountT idxCount      = static_cast<ElemCountT>(m_indices.size());
        const size_t     idxBufferSize = idxCount * sizeof(uint32_t);

        const size_t bufferSize = sizeof(ElemCountT) + vertBufferSize
            + sizeof(ElemCountT) + idxBufferSize;

        size_t offset = output.size();
        output.resize(output.size() + bufferSize);

        ElemCountT beCount = toBigEndian(vertCount);
        if (memcpy_s(output.data() + offset, output.size() - offset, &beCount, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to write vertex count to memory buffer");
            output.resize(output.size() - bufferSize);
            return false;
        }

        {
            std::vector<Vertex> beVertices(m_vertices);

            for (Vertex& beVertex : beVertices)
                vertexToBigEndian(beVertex);

            offset += sizeof(ElemCountT);
            if (memcpy_s(output.data() + offset, output.size() - offset, beVertices.data(), vertBufferSize) != 0)
            {
                DEBUG_LOG_ERROR("Unable to write vertices to memory buffer");
                output.resize(output.size() - bufferSize);
                return false;
            }
        }

        offset += vertBufferSize;
        beCount = toBigEndian(idxCount);
        if (memcpy_s(output.data() + offset, output.size() - offset, &beCount, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to write index count to memory buffer");
            output.resize(output.size() - bufferSize);
            return false;
        }

        {
            std::vector<uint32_t> beIndices(m_indices);

            for (uint32_t& beIndex : beIndices)
                beIndex = toBigEndian(beIndex);

            offset += sizeof(ElemCountT);
            if (memcpy_s(output.data() + offset, output.size() - offset, beIndices.data(), idxBufferSize) != 0)
            {
                DEBUG_LOG_ERROR("Unable to write indices to memory buffer");
                output.resize(output.size() - bufferSize);
                return false;
            }
        }

        return true;
    }

    size_t Mesh::fromBinary(const char* data, const size_t length)
    {
        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize mesh - Empty buffer");
            return 0;
        }

        const char* buffer = static_cast<const char*>(data);

        m_vertices.clear();
        m_indices.clear();

        ElemCountT elemCount;
        if (length < sizeof(ElemCountT) || memcpy_s(&elemCount, sizeof(ElemCountT), buffer, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to load vertex count from memory buffer");
            return 0;
        }

        size_t bufferSize = fromBigEndian(elemCount) * sizeof(Vertex);
        m_vertices.resize(elemCount);

        size_t offset = sizeof(ElemCountT);
        if (length <= offset || length - offset < bufferSize || memcpy_s(m_vertices.data(), bufferSize, buffer + offset,
            bufferSize) != 0)
        {
            DEBUG_LOG_ERROR("Unable to load vertices from memory buffer");
            return 0;
        }

        m_boundingBox =
        {
            Vector3(std::numeric_limits<float>::max()),
            Vector3(std::numeric_limits<float>::lowest())
        };

        for (Vertex& vertex : m_vertices)
        {
            vertexFromBigEndian(vertex);

            m_boundingBox.m_min = min(m_boundingBox.m_min, vertex.m_position);
            m_boundingBox.m_max = max(m_boundingBox.m_max, vertex.m_position);
        }

        offset += bufferSize;

        if (length <= offset || length - offset < sizeof(ElemCountT) || memcpy_s(&elemCount, sizeof(ElemCountT), buffer + offset,
            sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to load index count from memory buffer");
            return 0;
        }

        bufferSize = fromBigEndian(elemCount) * sizeof(uint32_t);

        if (length <= offset || length - offset < bufferSize || memcpy_s(m_indices.data(), bufferSize, buffer + offset,
            bufferSize) != 0)
        {
            DEBUG_LOG_ERROR("Unable to load indices from memory buffer");
            return 0;
        }

        for (uint32_t& index : m_indices)
            index = fromBigEndian(index);

        return offset + bufferSize;
    }

    bool Mesh::init()
    {
        m_vbo = IVertexBuffer::create(m_vertices);
        m_ebo = IIndexBuffer::create(m_indices);
        m_vao = IVertexArray::create(*m_vbo, *m_ebo);

        return true;
    }

    void Mesh::bind() const
    {
        m_vao->bind();
    }

    void Mesh::unbind() const
    {
        m_vao->unbind();
    }

    uint32_t Mesh::getMaterialIndex() const
    {
        return m_materialIndex;
    }

    uint32_t Mesh::getIndexCount() const
    {
        return static_cast<uint32_t>(m_indices.size());
    }

    BoundingBox Mesh::getBoundingBox() const
    {
        return m_boundingBox;
    }

    EPrimitiveType Mesh::getPrimitiveType() const
    {
        return m_primitiveType;
    }

    void Mesh::vertexToBigEndian(Vertex& vertex)
    {
        vec3ToBigEndian(vertex.m_position);
        vec3ToBigEndian(vertex.m_normal);
        vec2ToBigEndian(vertex.m_textureUV);
        vec3ToBigEndian(vertex.m_tangent);
        vec3ToBigEndian(vertex.m_bitangent);
    }

    void Mesh::vertexFromBigEndian(Vertex& vertex)
    {
        vec3FromBigEndian(vertex.m_position);
        vec3FromBigEndian(vertex.m_normal);
        vec2FromBigEndian(vertex.m_textureUV);
        vec3FromBigEndian(vertex.m_tangent);
        vec3FromBigEndian(vertex.m_bitangent);
    }
}
