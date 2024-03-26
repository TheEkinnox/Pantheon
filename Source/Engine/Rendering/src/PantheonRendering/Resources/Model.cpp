#include "PantheonRendering/Resources/Model.h"

#include "PantheonRendering/LowRenderer/Camera.h"
#include "PantheonRendering/Resources/Mesh.h"

#include <PantheonCore/Resources/ResourceRef.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>
#include <assimp/scene.h>

using namespace LibMath;
using namespace PantheonCore::Resources;
using namespace PantheonCore::Utility;
using namespace PantheonRendering::Geometry;

namespace PantheonRendering::Resources
{
    Model::Model()
        : Model({}, {})
    {
    }

    Model::Model(std::vector<Mesh> meshes, const size_t materialsCount)
        : m_meshes(std::move(meshes)), m_materialCount(materialsCount)
    {
        m_boundingBox =
        {
            Vector3(std::numeric_limits<float>::max()),
            Vector3(std::numeric_limits<float>::lowest())
        };

        for (const Mesh& mesh : m_meshes)
        {
            m_boundingBox.m_min = min(m_boundingBox.m_min, mesh.getBoundingBox().m_min);
            m_boundingBox.m_max = max(m_boundingBox.m_max, mesh.getBoundingBox().m_max);
        }
    }

    bool Model::load(const std::string& fileName)
    {
        Assimp::Importer importer;
        importer.SetPropertyInteger(AI_CONFIG_PP_SBP_REMOVE,
            aiPrimitiveType_POINT | aiPrimitiveType_LINE | aiPrimitiveType_POLYGON);

        const aiScene* scene = importer.ReadFile(fileName.c_str(), aiProcess_Triangulate | aiProcess_JoinIdenticalVertices
            | aiProcess_SortByPType | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph | aiProcess_RemoveRedundantMaterials
            | aiProcess_ImproveCacheLocality | aiProcess_GenUVCoords | aiProcess_GenSmoothNormals | aiProcess_CalcTangentSpace);

        if (!CHECK(scene && scene->HasMeshes(), "Unable to load model from file at path \"%s\"", fileName.c_str()))
            return false;

        m_meshes.clear();
        m_meshes.reserve(scene->mNumMeshes);
        m_materialCount = 0;

        m_boundingBox =
        {
            Vector3(std::numeric_limits<float>::max()),
            Vector3(std::numeric_limits<float>::lowest())
        };

        // Ensure assimp vec3s are compatible with ours so we can safely convert from one to the other
        static_assert(sizeof(aiVector3D) == sizeof(Vector3));
        static_assert(
            offsetof(aiVector3D, x) == offsetof(Vector3, m_x) &&
            offsetof(aiVector3D, y) == offsetof(Vector3, m_y) &&
            offsetof(aiVector3D, z) == offsetof(Vector3, m_z)
        );

        for (unsigned int i = 0; i < scene->mNumMeshes; ++i)
        {
            const aiMesh* mesh = scene->mMeshes[i];

            if (!mesh || mesh->mNumVertices == 0)
                continue;

            std::vector<Vertex> vertices;
            vertices.reserve(vertices.size() + mesh->mNumVertices);

            for (unsigned int idx = 0; idx < mesh->mNumVertices; ++idx)
            {
                const Vector3    position  = *reinterpret_cast<Vector3*>(&mesh->mVertices[idx]);
                const Vector3    normal    = mesh->mNormals ? *reinterpret_cast<Vector3*>(&mesh->mNormals[idx]) : Vector3(0);
                const aiVector3D uv        = mesh->mTextureCoords[0] ? mesh->mTextureCoords[0][idx] : aiVector3D(0);
                const Vector3    tangent   = mesh->mTangents ? *reinterpret_cast<Vector3*>(&mesh->mTangents[idx]) : Vector3(0);
                const Vector3    bitangent = mesh->mBitangents ? *reinterpret_cast<Vector3*>(&mesh->mBitangents[idx]) : Vector3(0);

                vertices.emplace_back(position, normal, Vector2(uv.x, uv.y), tangent, bitangent);
            }

            std::vector<uint32_t> indices;
            indices.reserve(static_cast<size_t>(mesh->mNumFaces) * 3);
            for (unsigned int faceIdx = 0; faceIdx < mesh->mNumFaces; ++faceIdx)
            {
                const aiFace face = mesh->mFaces[faceIdx];

                indices.push_back(face.mIndices[0]);
                indices.push_back(face.mIndices[1]);
                indices.push_back(face.mIndices[2]);
            }

            const Mesh& newMesh = m_meshes.emplace_back(vertices, indices, Enums::EPrimitiveType::TRIANGLES, mesh->mMaterialIndex);
            m_boundingBox.m_min = min(m_boundingBox.m_min, newMesh.getBoundingBox().m_min);
            m_boundingBox.m_max = max(m_boundingBox.m_max, newMesh.getBoundingBox().m_max);
        }

        m_materialCount = scene->mNumMaterials;

        return true;
    }

    bool Model::init()
    {
        for (auto& mesh : m_meshes)
        {
            if (!mesh.init())
                return false;
        }

        return true;
    }

    bool Model::toBinary(std::vector<char>& output) const
    {
        return serializeMeshes(output) && CHECK(writeNumber(static_cast<ElemCountT>(m_materialCount), output),
                "Unable to serialize model - Material count serialization failed");
    }

    size_t Model::fromBinary(const char* data, const size_t length)
    {
        const size_t meshBytes = deserializeMeshes(data, length);

        if (meshBytes == 0)
            return 0;

        const size_t materialCountBytes = readNumber<size_t, ElemCountT>(m_materialCount, data + meshBytes, length - meshBytes);

        if (!CHECK(materialCountBytes != 0, "Unable to deserialize model - Couldn't read material count"))
            return 0;

        return meshBytes + materialCountBytes;
    }

    const Mesh& Model::getMesh(const size_t index) const
    {
        ASSERT(index < m_meshes.size());
        return m_meshes[index];
    }

    size_t Model::getMaterialIndex(const size_t meshIndex) const
    {
        if (meshIndex >= m_meshes.size())
            return INVALID_INDEX;

        return m_meshes[meshIndex].getMaterialIndex();
    }

    size_t Model::getMeshCount() const
    {
        return m_meshes.size();
    }

    size_t Model::getMaterialCount() const
    {
        return m_materialCount;
    }

    BoundingBox Model::getBoundingBox() const
    {
        return m_boundingBox;
    }

    bool Model::serializeMeshes(std::vector<char>& output) const
    {
        const size_t offset = output.size();
        output.resize(output.size() + sizeof(ElemCountT));

        const ElemCountT meshCount = toBigEndian(static_cast<ElemCountT>(m_meshes.size()));
        if (memcpy_s(output.data() + offset, output.size() - offset, &meshCount, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to serialize model - Mesh count serialization failed");
            return false;
        }

        for (const Mesh& mesh : m_meshes)
        {
            if (!CHECK(mesh.toBinary(output), "Unable to serialize model"))
                return false;
        }

        return true;
    }

    size_t Model::deserializeMeshes(const char* data, const size_t length)
    {
        if (!CHECK(data != nullptr && length >= sizeof(ElemCountT), "Unable to deserialize model - Couldn't read meshes"))
            return 0;

        ElemCountT elemCount;
        size_t     offset = readNumber(elemCount, data, length);

        if (!CHECK(offset != 0, "Unable to deserialize model - Couldn't read mesh count"))
            return 0;

        m_meshes.resize(elemCount);

        for (ElemCountT i = 0; i < elemCount; ++i)
        {
            const size_t readBytes = m_meshes[i].fromBinary(data + offset, length - offset);

            if (readBytes == 0)
                return 0;

            offset += readBytes;
        }

        return offset;
    }
}
