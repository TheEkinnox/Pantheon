#include "PantheonCore/ECS/Entity.h"

#include "PantheonCore/Debug/Logger.h"

namespace PantheonCore::ECS
{
    Entity::Entity()
        : Entity(nullptr)
    {
    }

    Entity::Entity(Entity* parent)
        : Node(parent), m_isActive(true), m_isDestroyed(false)
    {
    }

    Entity::Entity(const Entity& other)
        : Node(other), m_components(other.m_components), m_isActive(other.m_isActive), m_isDestroyed(false)
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;
    }

    Entity::Entity(Entity&& other) noexcept
        : Node(std::forward<Node&&>(other)), m_components(std::move(other.m_components)), m_isActive(other.m_isActive),
        m_isDestroyed(false)
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;
    }

    Entity::~Entity()
    {
        m_isDestroyed = true;

        if (!m_components.empty())
            m_components.clear();
    }

    Entity::operator bool() const
    {
        return !m_isDestroyed;
    }

    Entity& Entity::operator=(const Entity& other)
    {
        if (&other == this)
            return *this;

        Node::operator=(other);

        m_components.clear();

        m_components = other.m_components;

        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;

        m_isActive = other.m_isActive;

        return *this;
    }

    Entity& Entity::operator=(Entity&& other) noexcept
    {
        if (&other == this)
            return *this;

        Node::operator=(other);

        m_components.clear();

        m_components = std::move(other.m_components);

        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;

        m_isActive = other.m_isActive;

        return *this;
    }

    void Entity::removeComponent(const Component& component)
    {
        removeComponent(component.getId());
    }

    void Entity::removeComponent(const Component::ComponentId id)
    {
        if (m_components.empty())
            return;

        const auto findFunc = [id](Component* ptr)
        {
            if (ptr && ptr->getId() == id)
            {
                ptr->onRemove();
                ptr->m_owner = nullptr;
                delete ptr;
                return true;
            }

            return false;
        };

        std::erase_if(m_components, findFunc);
    }

    void Entity::update()
    {
        if (!isActive())
            return;

        for (const auto& component : m_components)
        {
            if (component->isActive())
                component->update();
        }

        for (NodePtr& child : getChildren())
            reinterpret_cast<Entity&>(*child).update();
    }

    bool Entity::isActive() const
    {
        const auto* parent = reinterpret_cast<const Entity*>(Node::getParent());
        return !m_isDestroyed && m_isActive && (parent == nullptr || parent->isActive());
    }

    void Entity::setActive(const bool active)
    {
        m_isActive = active;
    }

    bool Entity::serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("components");
        writer.StartArray();

        for (const Component* component : m_components)
        {
            if (!component)
                continue;

            if (!serializeComponent(component, writer))
                return false;
        }

        writer.EndArray();

        writer.Key("children");
        writer.StartArray();

        for (ConstNodePtr& child : getChildren())
        {
            if (!CHECK(!child || reinterpret_cast<const Entity&>(*child).serialize(writer),
                    "Unable to serialize entity - Child serialization failed"))
                return false;
        }

        writer.EndArray();

        writer.EndObject();

        return true;
    }

    bool Entity::deserialize(const rapidjson::Value& json)
    {
        m_components.clear();
        clearChildren();

        if (!CHECK(json.IsObject(), "Unable to deserialize entity - Json value should be an object"))
            return false;

        auto it = json.FindMember("components");
        if (!CHECK(it != json.MemberEnd() && it->value.IsArray(), "Unable to deserialize entity - Invalid components array"))
            return false;

        for (const auto& jsonComponent : it->value.GetArray())
        {
            if (!CHECK(addComponentFromJson(jsonComponent),
                    "Unable to deserialize entity - Failed to deserialize component [%llu]", m_components.size()))
                return false;
        }

        it = json.FindMember("children");
        if (!CHECK(it != json.MemberEnd() && it->value.IsArray(), "Unable to deserialize entity - Invalid children array"))
            return false;

        for (const rapidjson::Value& jsonChild : it->value.GetArray())
        {
            if (!CHECK(addChild<Entity>(this).deserialize(jsonChild),
                    "Unable to deserialize entity - Failed to deserialize child [%llu]", getChildren().size() - 1))
                return false;
        }

        return true;
    }

    bool Entity::serialize(std::vector<char>& output) const
    {
        size_t offset = output.size();
        output.resize(offset + sizeof(ElemCountT));

        ElemCountT elemCount   = static_cast<ElemCountT>(m_components.size());
        ElemCountT beElemCount = Utility::toBigEndian(elemCount);
        if (!CHECK(memcpy_s(output.data() + offset, output.size() - offset, &beElemCount, sizeof(ElemCountT)) == 0,
                "Unable to write entity component count to memory buffer"))
            return false;

        for (const Component* component : m_components)
        {
            if (!serializeComponent(component, output))
                return false;
        }

        const std::vector<ConstNodePtr>& children = getChildren();

        offset = output.size();
        output.resize(offset + sizeof(ElemCountT));

        elemCount   = static_cast<ElemCountT>(children.size());
        beElemCount = Utility::toBigEndian(elemCount);
        if (!CHECK(memcpy_s(output.data() + offset, output.size() - offset, &beElemCount, sizeof(ElemCountT)) == 0,
                "Unable to write entity child count to memory buffer"))
            return false;

        for (const ConstNodePtr& child : children)
        {
            if (!CHECK(!child || reinterpret_cast<const Entity&>(*child).serializeWithSize(output),
                    "Unable to serialize entity - Child serialization failed"))
                return false;
        }

        return true;
    }

    size_t Entity::deserialize(const void* data, const size_t length)
    {
        m_components.clear();
        clearChildren();

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize entity - Invalid buffer"))
            return 0;

        const char* charData  = static_cast<const char*>(data);
        ElemCountT  elemCount = 0;

        if (!CHECK(length > sizeof(ElemCountT) && memcpy_s(&elemCount, sizeof(ElemCountT), charData, sizeof(ElemCountT)) == 0,
                "Unable to deserialize entity - Failed to read component count"))
            return 0;

        size_t offset = sizeof(ElemCountT);
        for (ElemCountT i = 0; i < Utility::fromBigEndian(elemCount); ++i)
        {
            if (!CHECK(length > offset, "Unable to deserialize entity - Invalid offset"))
                return 0;

            const size_t readBytes = addComponentFromMemory(charData + offset, length - offset);
            if (!CHECK(readBytes != 0, "Unable to deserialize entity - Failed to deserialize component [%d]", i))
                return 0;

            offset += readBytes;
        }

        elemCount = 0;
        if (!CHECK(length > offset && length - offset >= sizeof(ElemCountT)
                && memcpy_s(&elemCount, sizeof(ElemCountT), charData + offset, sizeof(ElemCountT)) == 0,
                "Unable to deserialize entity - Failed to read child count"))
            return 0;

        offset += sizeof(ElemCountT);
        for (ElemCountT i = 0; i < Utility::fromBigEndian(elemCount); ++i)
        {
            if (!CHECK(length > offset, "Unable to deserialize entity - Invalid offset"))
                return 0;

            const size_t readBytes = addChildFromMemory(charData + offset, length - offset);
            if (!CHECK(readBytes != 0, "Unable to deserialize entity - Failed to deserialize child [%d]", i))
                return 0;

            offset += readBytes;
        }

        return offset;
    }

    void Entity::onChildAdded(Node& child)
    {
        m_childAddedEvent.invoke(reinterpret_cast<Entity&>(child));
    }

    void Entity::onRemoveChild(Node& child)
    {
        m_childRemovedEvent.invoke(reinterpret_cast<Entity&>(child));
    }

    bool Entity::serializeComponent(const Component* component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        const std::string typeName = component->getTypeName();

        writer.Key("type");
        writer.String(typeName.c_str(), static_cast<rapidjson::SizeType>(typeName.size()));

        writer.Key("data");
        if (!CHECK(((IJsonSerializable&)*component).serialize(writer),
                "Unable to serialize entity - Component serialization failed"))
            return false;

        writer.EndObject();

        return true;
    }

    bool Entity::addComponentFromJson(const rapidjson::Value& jsonComponent)
    {
        if (!CHECK(jsonComponent.IsObject(), "Unable to deserialize entity component - Json value should be an object"))
            return false;

        auto it = jsonComponent.FindMember("type");
        if (!CHECK(it != jsonComponent.MemberEnd() && it->value.IsString(),
                "Failed to deserialize entity component - Invalid type string"))
            return false;

        const std::string type = it->value.GetString();

        it = jsonComponent.FindMember("data");
        if (!CHECK(it != jsonComponent.MemberEnd() && it->value.IsObject(),
                "Failed to deserialize entity component - Invalid component data"))
            return false;

        Component* component = Component::create(type, *this);
        if (!component || !((IJsonSerializable&)*component).deserialize(it->value))
            return false;

        m_components.push_back(component);
        return true;
    }

    bool Entity::serializeComponent(const Component* component, std::vector<char>& output)
    {
        if (!CHECK(serializeString<uint8_t>(component->getTypeName(), output),
                "Unable to write entity to memory buffer - Failed to write component type string"))
            return false;

        if (!CHECK(component->serializeWithSize(output), "Unable to write entity to memory buffer - Failed to serialize component"))
            return false;

        return true;
    }

    size_t Entity::addComponentFromMemory(const char* data, const size_t length)
    {
        const size_t componentIdx = m_components.size();

        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize entity component [%d] - Invalid buffer", componentIdx))
            return 0;

        std::string type;
        if (!CHECK(deserializeString<uint8_t>(type, data, length) != 0,
                "Unable to deserialize entity component [%d] - Failed to read type string", componentIdx))
            return 0;

        size_t offset = sizeof(uint8_t) + type.size();
        if (!CHECK(length > offset && length - offset >= sizeof(ElemSizeT),
                "Unable to deserialize entity component [%d] - Invalid offset", componentIdx))
            return 0;

        const ElemSizeT elemSize = readNumber<ElemSizeT>(data + offset, length - offset);

        if (!CHECK(elemSize != INVALID_ELEMENT_SIZE,
                "Unable to deserialize entity component [%d] - Failed to read component's buffer size", componentIdx))
            return 0;

        offset += sizeof(ElemSizeT);
        if (!CHECK(length > offset && length - offset >= elemSize,
                "Unable to deserialize entity component [%d] - Invalid offset", componentIdx))
            return 0;

        Component* component = Component::create(type, *this);
        if (!component || ((IByteSerializable&)*component).deserialize(data + offset, elemSize) == 0)
            return 0;

        m_components.push_back(component);
        return offset + elemSize;
    }

    size_t Entity::addChildFromMemory(const char* data, const size_t length)
    {
        if (!CHECK(data != nullptr && length == 0, "Unable to deserialize child entity - Invalid buffer"))
            return 0;

        const ElemSizeT elemSize = readNumber<ElemSizeT>(data, length);

        if (!CHECK(elemSize != INVALID_ELEMENT_SIZE, "Unable to deserialize child entity - Failed to read entity's buffer size"))
            return 0;

        if (!CHECK(length > sizeof(ElemSizeT) && length - sizeof(ElemSizeT) >= elemSize,
                "Unable to deserialize child entity - Invalid offset"))
            return 0;

        Entity& entity = addChild<Entity>(this);
        if (!CHECK(entity.deserialize(data + sizeof(ElemSizeT), elemSize) != 0,
                "Unable to deserialize entity - Failed to deserialize child [%llu]", getChildren().size() - 1))
            return 0;

        return sizeof(ElemSizeT) + elemSize;
    }
}
