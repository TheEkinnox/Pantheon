#include "PantheonRendering/Components/ModelComponent.h"

using namespace PantheonCore::Resources;
using namespace PantheonCore::Serialization;
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

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(
        const PantheonRendering::Components::ModelComponent& component, std::vector<char>& out, const EntitiesMap&)
    {
        if (!component.m_model.toBinary(out))
            return false;

        const auto materialCount = static_cast<IByteSerializable::ElemCountT>(component.m_materials.size());
        if (!CHECK(IByteSerializable::writeNumber(materialCount, out), "Unable to write model component's material count"))
            return false;

        for (IByteSerializable::ElemCountT i = 0; i < materialCount; ++i)
        {
            if (!component.m_materials[i].toBinary(out))
                return false;
        }

        return true;
    }

    template <>
    size_t ComponentRegistry::fromBinary(PantheonRendering::Components::ModelComponent& out, const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize model component - Empty buffer"))
            return 0;

        size_t offset = out.m_model.fromBinary(data, length);

        if (offset == 0 || !CHECK(length >= offset, "Unable to deserialize model component's material count - Invalid offset"))
            return 0;

        IByteSerializable::ElemCountT materialCount;

        size_t readBytes = IByteSerializable::readNumber(materialCount, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to deserialize model component's material count"))
            return 0;

        out.m_materials.reserve(materialCount);
        offset += readBytes;

        for (IByteSerializable::ElemCountT i = 0; i < materialCount; ++i)
        {
            if (!CHECK(length >= offset, "Unable to deserialize model component's material %u - Invalid offset", i))
                return 0;

            ResourceRef<Material>& mat = out.m_materials.emplace_back();

            readBytes = mat.fromBinary(data + offset, length - offset);

            if (readBytes == 0)
                return 0;

            offset += readBytes;
        }

        return offset;
    }

    template <>
    bool ComponentRegistry::toJson(
        const PantheonRendering::Components::ModelComponent& component, rapidjson::Writer<rapidjson::StringBuffer>& writer,
        const EntitiesMap&)
    {
        writer.StartObject();

        writer.Key("model");
        if (!component.m_model.toJson(writer))
            return false;

        writer.Key("materials");

        writer.StartArray();

        for (const ResourceRef<Material>& material : component.m_materials)
        {
            if (!material.toJson(writer))
                return false;
        }

        writer.EndArray();

        return writer.EndObject();
    }

    template <>
    bool ComponentRegistry::fromJson(PantheonRendering::Components::ModelComponent& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize model component - Json value should be an object"))
            return false;

        auto it = json.FindMember("model");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize model component's model"))
            return false;

        if (!out.m_model.fromJson(it->value))
            return false;

        it = json.FindMember("materials");

        if (!CHECK(it != json.MemberEnd() && it->value.IsArray(),
                "Unable to deserialize model component's materials - Json value should be an array"))
            return false;

        const auto jsonMaterials = it->value.GetArray();
        out.m_materials.reserve(jsonMaterials.Size());

        for (const auto& jsonMaterial : jsonMaterials)
        {
            ResourceRef<Material>& mat = out.m_materials.emplace_back();

            if (!mat.fromJson(jsonMaterial))
                return false;
        }

        return true;
    }
}
