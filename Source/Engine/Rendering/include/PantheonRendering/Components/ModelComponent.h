#pragma once
#include "PantheonRendering/LowRenderer/Camera.h"
#include "PantheonRendering/Resources/Model.h"

#include <PantheonCore/ECS/ComponentRegistry.h>

namespace PantheonRendering::Components
{
    class ModelComponent
    {
    public:
        using ModelRef = PantheonCore::Resources::ResourceRef<Resources::Model>;
        using MaterialRef = PantheonCore::Resources::ResourceRef<Resources::Material>;
        using MaterialList = std::vector<MaterialRef>;

        /**
         * \brief Creates a default model component
         */
        ModelComponent();

        /**
         * \brief Creates a model component with the given model, materials and layer mask
         * \param model The component's model
         * \param materials The component's materials
         * \param layerMask The component's layer mask
         */
        ModelComponent(const ModelRef& model, const MaterialList& materials, Core::LayerMask layerMask);

        /**
         * \brief Creates a copy of the given model component
         * \param other The component to copy
         */
        ModelComponent(const ModelComponent& other) = default;

        /**
         * \brief Creates a move copy of the given model component
         * \param other The component to move
         */
        ModelComponent(ModelComponent&& other) noexcept = default;

        /**
         * \brief Destroys the model component
         */
        ~ModelComponent() = default;

        /**
         * \brief Assigns a copy of the given model component to this one
         * \param other The model component to copy
         * \return A reference to the modified component
         */
        ModelComponent& operator=(const ModelComponent& other) = default;

        /**
         * \brief Moves the given model component into this one
         * \param other The model component to move
         * \return A reference to the modified component
         */
        ModelComponent& operator=(ModelComponent&& other) noexcept = default;

        /**
         * \brief Dereferences the component's model
         * \return A pointer to the component's model
         */
        Resources::Model* operator*();

        /**
         * \brief Dereferences the component's model
         * \return A constant pointer to the component's model
         */
        const Resources::Model* operator*() const;

        /**
         * \brief Dereferences the component's model
         * \return A pointer to the component's model
         */
        Resources::Model* operator->();

        /**
         * \brief Dereferences the component's model
         * \return A constant pointer to the component's model
         */
        const Resources::Model* operator->() const;

        /**
         * \brief Sets the component's model reference
         * \param model The component's new model resource
         */
        void setModel(ModelRef model);

        /**
         * \brief Gets the model's local material instance at the given index
         * \return The model's material instance
         * \param index The target material's index
         */
        Resources::Material& getMaterial(size_t index);

        /**
         * \brief Gets the model's local material instance at the given index
         * \return The model's material instance
         * \param index The target material's index
         */
        const Resources::Material& getMaterial(size_t index) const;

        /**
         * \brief Creates an instance of the model's shared material at the given index
         * \return The model's material instance
         * \param index The target material's index
         */
        Resources::Material& instantiateMaterial(size_t index);

        /**
         * \brief Resets the model's material instance at the given index
         * \param index The target material's index
         */
        void resetMaterialInstance(size_t index);

        /**
         * \brief Gets the model's shared material resource at the given index
         * \return The model's shared material resource
         * \param index The target material's index
         */
        Resources::Material& getSharedMaterial(size_t index) const;

        /**
         * \brief Sets the model's shared material resource at the given index
         * \param material The model's new material's resource reference
         * \param index The target material's index
         */
        void setSharedMaterial(size_t index, const MaterialRef& material);

        /**
         * \brief Gets the model's layer mask
         * \return The model's layer mask
         */
        Core::LayerMask getLayerMask() const;

        /**
         * \brief Sets the model's layer mask
         * \param layerMask The model's new layer mask
         */
        void setLayerMask(Core::LayerMask layerMask);

    private:
        friend class PantheonCore::ECS::ComponentRegistry;

        ModelRef                                        m_model;
        std::vector<MaterialRef>                        m_materials;
        std::vector<std::optional<Resources::Material>> m_materialInstances;

        Core::LayerMask m_layerMask;
    };
}

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(
        const PantheonRendering::Components::ModelComponent& component, std::vector<char>& out, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(
        PantheonRendering::Components::ModelComponent& out, const char* data, size_t length, Scene*);

    template <>
    bool ComponentRegistry::toJson(
        const PantheonRendering::Components::ModelComponent& component, rapidjson::Writer<rapidjson::StringBuffer>& writer,
        const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(PantheonRendering::Components::ModelComponent& out, const rapidjson::Value& json, Scene*);
}
