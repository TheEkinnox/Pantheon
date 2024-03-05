#pragma once
#include "PantheonRendering/Resources/Material.h"
#include "PantheonRendering/Resources/Mesh.h"

#include <PantheonCore/Resources/IResource.h>
#include <PantheonCore/Resources/ResourceRef.h>

namespace PantheonRendering::Resources
{
    class Model final : public PantheonCore::Resources::IResource
    {
        REGISTERED_RESOURCE_BODY(Model)

    public:
        static constexpr size_t INVALID_INDEX = static_cast<size_t>(-1);

        /**
         * \brief Creates an empty model
         */
        Model();

        /**
         * \brief Creates a model with the given meshes and number of materials
         * \param meshes The model's meshes
         * \param materialsCount The number of materials used by this model
         */
        Model(std::vector<Mesh> meshes, size_t materialsCount);

        Model(const Model& other)     = default;
        Model(Model&& other) noexcept = default;

        ~Model() override = default;

        Model& operator=(const Model& other)     = default;
        Model& operator=(Model&& other) noexcept = default;

        /**
         * \brief Loads the model from the given file
         * \param fileName The path of the model to load
         * \return True if the model was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        /**
         * \brief Initializes the model
         * \return True if the model was successfully initialized. False otherwise.
         */
        bool init() override;

        /**
         * \brief Serializes the model to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the model from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

        /**
         * \brief Gets the model's mesh at the given index
         * \return The model's mesh at the given index
         * \param index The target mesh's index
         */
        const Mesh& getMesh(size_t index) const;

        /**
         * \brief Gets the material index for the mesh at the given index
         * \param meshIndex The target mesh index
         * \return The mesh's material index on success. INVALID_INDEX otherwise
         */
        size_t getMaterialIndex(size_t meshIndex) const;

        /**
         * \brief Gets the number of meshes composing the model
         * \return The model's mesh count
         */
        size_t getMeshCount() const;

        /**
         * \brief Gets the number of unique materials used by the model
         * \return The model's material count
         */
        size_t getMaterialCount() const;

        /**
         * \brief Gets the mesh's bounding box
         * \return The mesh's bounding box
         */
        Geometry::BoundingBox getBoundingBox() const;

    private:
        std::vector<Mesh>     m_meshes;
        size_t                m_materialsCount;
        Geometry::BoundingBox m_boundingBox;

        bool   serializeMeshes(std::vector<char>& output) const;
        size_t deserializeMeshes(const char* data, size_t length);

        bool   serializeMaterialCount(std::vector<char>& output) const;
        size_t deserializeMaterialCount(const char* data, size_t length);
    };
}
