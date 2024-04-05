#include "PantheonRendering/Components/ModelComponent.h"

using namespace PantheonRendering::Core;
using namespace PantheonRendering::Resources;

namespace PantheonRendering::Components
{
    ModelComponent::ModelComponent()
        : ModelComponent({}, {}, Layer::ALL)
    {
    }

    ModelComponent::ModelComponent(const ModelRef& model, const MaterialList& materials, const LayerMask layerMask)
        : m_layerMask(layerMask)
    {
        setModel(model);

        for (size_t i = 0; i < materials.size(); ++i)
            setSharedMaterial(i, materials[i]);
    }

    Model* ModelComponent::operator*()
    {
        return m_model.getOrDefault();
    }

    const Model* ModelComponent::operator*() const
    {
        return m_model.getOrDefault();
    }

    Model* ModelComponent::operator->()
    {
        return m_model.getOrDefault();
    }

    const Model* ModelComponent::operator->() const
    {
        return m_model.getOrDefault();
    }

    void ModelComponent::setModel(ModelRef model)
    {
        m_model = std::move(model);
        m_materials.clear();
        m_materialInstances.clear();

        const Model* modelResource = m_model.getOrDefault();

        if (!modelResource)
            return;

        m_materials.resize(modelResource->getMaterialCount());
        m_materialInstances.resize(m_materials.size());
    }

    Material& ModelComponent::getMaterial(const size_t index)
    {
        ASSERT(index < m_materialInstances.size());
        std::optional<Material>& instance = m_materialInstances[index];

        return instance.has_value() ? instance.value() : getSharedMaterial(index);
    }

    const Material& ModelComponent::getMaterial(const size_t index) const
    {
        ASSERT(index < m_materialInstances.size());
        const std::optional<Material>& instance = m_materialInstances[index];

        return instance.has_value() ? instance.value() : getSharedMaterial(index);
    }

    Material& ModelComponent::instantiateMaterial(const size_t index)
    {
        ASSERT(index < m_materials.size());

        m_materialInstances[index] = getSharedMaterial(index);
        return m_materialInstances[index].value();
    }

    void ModelComponent::resetMaterialInstance(const size_t index)
    {
        ASSERT(index < m_materialInstances.size());
        m_materialInstances[index] = std::nullopt;
    }

    Material& ModelComponent::getSharedMaterial(const size_t index) const
    {
        ASSERT(index < m_materials.size());

        static Material defaultMat;
        Material*       ptr = m_materials[index].getOrDefault();

        return ptr ? *ptr : defaultMat;
    }

    void ModelComponent::setSharedMaterial(const size_t index, const MaterialRef& material)
    {
        ASSERT(index < m_materials.size());

        m_materials[index] = material;
        resetMaterialInstance(index);
    }

    LayerMask ModelComponent::getLayerMask() const
    {
        return m_layerMask;
    }

    void ModelComponent::setLayerMask(LayerMask layerMask)
    {
        m_layerMask = layerMask;
    }
}
