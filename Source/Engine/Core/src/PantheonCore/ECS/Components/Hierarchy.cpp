#include "PantheonCore/ECS/Components/Hierarchy.h"

#include "PantheonCore/ECS/SceneView.h"

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonCore::ECS
{
    HierarchyComponent::HierarchyComponent(const Entity parent)
        : m_parent(parent)
    {
    }

    Entity HierarchyComponent::getParent() const
    {
        return m_parent;
    }

    void HierarchyComponent::setParent(const Entity parent)
    {
        m_parent = parent;
    }

    Entity HierarchyComponent::getFirstChild() const
    {
        return m_firstChild;
    }

    Entity HierarchyComponent::getPreviousSibling() const
    {
        return m_previousSibling;
    }

    Entity HierarchyComponent::getNextSibling() const
    {
        return m_nextSibling;
    }

    size_t HierarchyComponent::getChildCount() const
    {
        return m_childCount;
    }

    void LinkTransforms(EntityHandle entity)
    {
        Transform* parentTransform = entity.getParent().getInParent<Transform>();
        Transform* transform       = entity.get<Transform>();

        if (transform)
            transform->setParent(parentTransform, parentTransform == nullptr);

        const std::vector<Transform*> childTransforms = GetChildTransforms(entity);

        for (Transform* childTransform : childTransforms)
        {
            Transform* newParent = transform ? transform : parentTransform;
            childTransform->setParent(newParent, parentTransform == nullptr);
        }
    }

    void UnlinkTransforms(const EntityHandle entity)
    {
        Transform* parentTransform = entity.getParent().getInParent<Transform>();

        const std::vector<Transform*> linkedTransforms = GetChildTransforms(entity);

        for (Transform* transform : linkedTransforms)
            transform->setParent(parentTransform, true);
    }

    std::vector<Transform*> GetChildTransforms(const EntityHandle entity)
    {
        std::vector<Transform*> linkedTransforms;

        const std::vector<EntityHandle> children = entity.getChildren();

        for (EntityHandle child : children)
        {
            if (Transform* childTransform = child.get<Transform>())
            {
                linkedTransforms.push_back(childTransform);
            }
            else
            {
                const std::vector<Transform*> childTransforms = GetChildTransforms(child);
                linkedTransforms.insert(linkedTransforms.end(), childTransforms.begin(), childTransforms.end());
            }
        }

        return linkedTransforms;
    }

    template <>
    void ComponentTraits::onAdd<HierarchyComponent>(const EntityHandle owner, HierarchyComponent& hierarchy)
    {
        ASSERT(hierarchy.m_firstChild == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        ASSERT(hierarchy.m_previousSibling == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        ASSERT(hierarchy.m_nextSibling == NULL_ENTITY, "Adding a pre-existing hierarchy is not supported");
        ASSERT(hierarchy.m_childCount == 0, "Adding a pre-existing hierarchy is not supported");

        onChange(owner, hierarchy);
    }

    template <>
    void ComponentTraits::onRemove<HierarchyComponent>(const EntityHandle entity, HierarchyComponent& hierarchy)
    {
        onBeforeChange(entity, hierarchy);

        EntityHandle child(entity.getScene(), hierarchy.m_firstChild);

        hierarchy.m_parent          = NULL_ENTITY;
        hierarchy.m_nextSibling     = NULL_ENTITY;
        hierarchy.m_previousSibling = NULL_ENTITY;

        while (child)
        {
            const EntityHandle nextChild = child.getNextSibling();
            child.destroy();
            child = nextChild;
        }

        hierarchy.m_childCount = 0;
        UnlinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onBeforeChange<HierarchyComponent>(const EntityHandle entity, HierarchyComponent& hierarchy)
    {
        Scene* scene = entity.getScene();
        ASSERT(scene);

        EntityHandle parent(scene, hierarchy.m_parent);
        EntityHandle nextSibling(scene, hierarchy.m_nextSibling);
        EntityHandle prevSibling(scene, hierarchy.m_previousSibling);

        if (HierarchyComponent* parentHierarchy = parent.get<HierarchyComponent>())
        {
            --parentHierarchy->m_childCount;

            if (entity.getEntity() == parentHierarchy->m_firstChild)
                parentHierarchy->m_firstChild = nextSibling;
        }

        if (HierarchyComponent* prevHierarchy = prevSibling.get<HierarchyComponent>())
            prevHierarchy->m_nextSibling = nextSibling;

        if (HierarchyComponent* nextHierarchy = nextSibling.get<HierarchyComponent>())
            nextHierarchy->m_previousSibling = prevSibling;

        hierarchy.m_parent          = NULL_ENTITY;
        hierarchy.m_nextSibling     = NULL_ENTITY;
        hierarchy.m_previousSibling = NULL_ENTITY;
    }

    template <>
    void ComponentTraits::onChange<HierarchyComponent>(const EntityHandle entity, HierarchyComponent& hierarchy)
    {
        Scene* scene = entity.getScene();
        ASSERT(scene);

        EntityHandle parent(scene, hierarchy.m_parent);

        if (EntityHandle firstChild(scene, hierarchy.m_firstChild); firstChild && !firstChild.has<HierarchyComponent>())
            firstChild.make<HierarchyComponent>(entity);

        if (!parent)
            return;

        if (HierarchyComponent* parentHierarchy = parent.get<HierarchyComponent>())
        {
            EntityHandle nextSibling(scene, hierarchy.m_nextSibling = parentHierarchy->m_firstChild);

            if (nextSibling)
                nextSibling.get<HierarchyComponent>()->m_previousSibling = entity;

            parentHierarchy->m_firstChild = entity;
            ++parentHierarchy->m_childCount;
        }
        else
        {
            parentHierarchy               = &parent.make<HierarchyComponent>(NULL_ENTITY);
            parentHierarchy->m_firstChild = entity;
            parentHierarchy->m_childCount = 1;
        }

        LinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onAdd<Transform>(const EntityHandle entity, Transform&)
    {
        LinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onRemove<Transform>(const EntityHandle entity, Transform&)
    {
        UnlinkTransforms(entity);
    }

    template <>
    void ComponentTraits::onChange<Transform>(const EntityHandle entity, Transform&)
    {
        LinkTransforms(entity);
    }

    template <>
    bool ComponentRegistry::toBinary<HierarchyComponent>(
        const HierarchyComponent& hierarchy, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        Entity parent = hierarchy.getParent();

        if (parent != NULL_ENTITY)
        {
            const auto it = toSerialized.find(parent);

            if (!CHECK(it != toSerialized.end(), "Unable to serialize hierarchy component - Parent is not serialized"))
                return false;

            parent = it->second;
        }

        return CHECK(IByteSerializable::writeNumber(parent, out), "Unable to serialize hierarchy's parent");
    }

    template <>
    size_t ComponentRegistry::fromBinary<HierarchyComponent>(HierarchyComponent& out, const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize hierarchy - Empty buffer"))
            return 0;

        return IByteSerializable::readNumber<Entity, Entity::Id>(out.m_parent, data, length);
    }

    template <>
    bool ComponentRegistry::toJson<HierarchyComponent>(
        const HierarchyComponent& hierarchy, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        Entity parent = hierarchy.getParent();

        if (parent != NULL_ENTITY)
        {
            const auto it = toSerialized.find(parent);

            if (!CHECK(it != toSerialized.end(), "Unable to serialize hierarchy component - Parent is not serialized"))
                return false;

            parent = it->second;
        }

        writer.StartObject();

        writer.Key("parent");
        writer.Uint64(parent);

        return writer.EndObject();
    }

    template <>
    bool ComponentRegistry::fromJson<HierarchyComponent>(HierarchyComponent& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize hierarchy - Json value should be an object"))
            return false;

        const auto it = json.FindMember("parent");
        if (!CHECK(it != json.MemberEnd() && it->value.Is<Entity::Id>(), "Unable to deserialize hierarchy - Invalid parent"))
            return false;

        out.setParent(Entity(it->value.Get<Entity::Id>()));
        return true;
    }

    template <>
    bool ComponentRegistry::toBinary<Transform>(const Transform& transform, std::vector<char>& out, const EntitiesMap&)
    {
        out.reserve(out.size() + sizeof(Vector3) * 2 + sizeof(Quaternion));

        if (!CHECK(IByteSerializable::serializeVector3(transform.getPosition(), out)))
            return false;

        if (!CHECK(IByteSerializable::serializeQuaternion(transform.getRotation(), out)))
            return false;

        if (!CHECK(IByteSerializable::serializeVector3(transform.getScale(), out)))
            return false;

        return true;
    }

    template <>
    size_t ComponentRegistry::fromBinary<Transform>(Transform& out, const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize transform - Empty buffer"))
            return 0;

        Vector3 position;
        size_t  offset = IByteSerializable::deserializeVector3(position, data, length);

        if (!CHECK(offset != 0, "Unable to deserialize transform position"))
            return 0;

        Quaternion rotation;
        size_t     readBytes = IByteSerializable::deserializeQuaternion(rotation, data + offset, length - offset);

        if (!CHECK(readBytes != 0, "Unable to deserialize transform rotation"))
            return 0;

        offset += readBytes;

        Vector3 scale;
        readBytes = IByteSerializable::deserializeVector3(scale, data + offset, length - offset);

        if (!CHECK(readBytes != 0, "Unable to deserialize transform scale"))
            return 0;

        out.setAll(position, rotation, scale);
        return offset + readBytes;
    }

    template <>
    bool ComponentRegistry::toJson<Transform>(
        const Transform& transform, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        writer.StartObject();

        writer.Key("position");
        std::string str = transform.getPosition().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.Key("rotation");
        str = transform.getRotation().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.Key("scale");
        str = transform.getScale().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        return writer.EndObject();
    }

    template <>
    bool ComponentRegistry::fromJson<Transform>(Transform& transform, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize transform - Invalid json object"))
            return false;

        Vector3    position, scale;
        Quaternion rotation;

        auto it = json.FindMember("position");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize transform position"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> position;
        }

        it = json.FindMember("rotation");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize transform rotation"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> rotation;
        }

        it = json.FindMember("scale");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize transform scale"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));
            iss >> scale;
        }

        transform.setAll(position, rotation, scale);
        return true;
    }
}
