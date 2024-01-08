#include "PantheonCore/Entities/Entity.h"

#include "PantheonCore/Debug/Logger.h"

using namespace LibMath;

namespace PantheonEngine::Core::Entities
{
    Entity::Entity() : Entity(nullptr, {})
    {
    }

    Entity::Entity(Entity* parent, const Transform& transform) :
        Node(parent), Transform(transform), m_isActive(true), m_isDestroyed(false)
    {
    }

    Entity::Entity(const Entity& other) :
        Node(other), Transform(other), m_components(other.m_components), m_isActive(other.m_isActive), m_isDestroyed(false)
    {
        if (!m_components.empty())
            for (const auto& component : m_components)
                component->m_owner = this;
    }

    Entity::Entity(Entity&& other) noexcept :
        Node(std::forward<Node&&>(other)), Transform(std::forward<Transform&&>(other)),
        m_components(std::move(other.m_components)), m_isActive(other.m_isActive), m_isDestroyed(false)
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
        Transform::operator=(other);

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
        Transform::operator=(other);

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

        const auto findFunc = [id](const ComponentPtr& ptr)
        {
            return ptr && ptr->getId() == id;
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

        writer.Key("transform");
        if (!serializeTransform(writer))
        {
            DEBUG_LOG_ERROR("Unable to serialize entity: Transform serialization failed");
            return false;
        }

        writer.Key("components");
        writer.StartArray();

        for (const ComponentPtr& component : m_components)
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
            if (child && !reinterpret_cast<const Entity&>(*child).serialize(writer))
            {
                DEBUG_LOG_ERROR("Unable to serialize entity - Child serialization failed");
                return false;
            }
        }

        writer.EndArray();

        writer.EndObject();

        return true;
    }

    bool Entity::deserialize(const rapidjson::Value& json)
    {
        m_components.clear();
        clearChildren();

        if (!json.IsObject())
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Json value should be an object");
            return false;
        }

        auto it = json.FindMember("transform");
        if (it == json.MemberEnd())
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Missing transform");
            return false;
        }

        if (!deserializeTransform(it->value))
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to deserialize transform");
            return false;
        }

        it = json.FindMember("components");
        if (it == json.MemberEnd() || !it->value.IsArray())
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Invalid components array");
            return false;
        }

        for (const auto& jsonComponent : it->value.GetArray())
        {
            if (!addComponentFromJson(jsonComponent))
            {
                DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to deserialize component [%llu]", m_components.size());
                return false;
            }
        }

        it = json.FindMember("children");
        if (it == json.MemberEnd() || !it->value.IsArray())
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Invalid children array");
            return false;
        }

        for (const rapidjson::Value& jsonChild : it->value.GetArray())
        {
            Entity& entity = addChild<Entity>(this, Transform());
            if (!entity.deserialize(jsonChild))
            {
                DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to deserialize child [%llu]", getChildren().size() - 1);
                return false;
            }
        }

        return true;
    }

    bool Entity::serialize(std::vector<char>& output) const
    {
        if (!serializeTransform(output))
            return false;

        size_t     offset = output.size();
        ElemCountT elemCount = static_cast<ElemCountT>(m_components.size());
        output.resize(offset + sizeof(ElemCountT));

        ElemCountT leElemCount = Utility::toBigEndian(elemCount);
        if (memcpy_s(output.data() + offset, output.size() - offset, &leElemCount, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to write entity component count to memory buffer");
            return false;
        }

        for (const ComponentPtr& component : m_components)
        {
            if (!serializeComponent(component, output))
                return false;
        }

        const std::vector<ConstNodePtr>& children = getChildren();
        elemCount = static_cast<ElemCountT>(children.size());

        offset = output.size();
        output.resize(offset + sizeof(ElemCountT));

        leElemCount = Utility::toBigEndian(elemCount);
        if (memcpy_s(output.data() + offset, output.size() - offset, &leElemCount, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to write entity child count to memory buffer");
            return false;
        }

        for (const ConstNodePtr& child : children)
        {
            if (child && !reinterpret_cast<const Entity&>(*child).serializeWithSize(output))
            {
                DEBUG_LOG_ERROR("Unable to serialize entity - Child serialization failed");
                return false;
            }
        }

        return true;
    }

    bool Entity::deserialize(const void* data, const size_t length)
    {
        m_components.clear();
        clearChildren();

        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Invalid buffer");
            return false;
        }

        const char* charData = static_cast<const char*>(data);

        if (!deserializeTransform(charData, length))
            return false;

        size_t     offset = sizeof(Vector3) * 2 + sizeof(Quaternion);
        ElemCountT elemCount = 0;
        if (length <= offset || length - offset < sizeof(ElemCountT)
            || memcpy_s(&elemCount, sizeof(ElemCountT), charData + offset, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to read component count");
            return false;
        }

        offset += sizeof(ElemCountT);
        for (ElemCountT i = 0; i < Utility::fromBigEndian(elemCount); ++i)
        {
            if (length <= offset)
            {
                DEBUG_LOG_ERROR("Unable to deserialize entity - Invalid offset");
                return false;
            }

            const size_t readBytes = addComponentFromMemory(charData + offset, length - offset);
            if (readBytes == 0)
            {
                DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to deserialize component [%d]", i);
                return false;
            }

            offset += readBytes;
        }

        elemCount = 0;
        if (length <= offset || length - offset < sizeof(ElemCountT)
            || memcpy_s(&elemCount, sizeof(ElemCountT), charData + offset, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to read child count");
            return false;
        }

        offset += sizeof(ElemCountT);
        for (ElemCountT i = 0; i < Utility::fromBigEndian(elemCount); ++i)
        {
            if (length <= offset)
            {
                DEBUG_LOG_ERROR("Unable to deserialize entity - Invalid offset");
                return false;
            }

            const size_t readBytes = addChildFromMemory(charData + offset, length - offset);
            if (readBytes == 0)
            {
                DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to deserialize child [%d]", i);
                return false;
            }

            offset += readBytes;
        }

        return true;
    }

    void Entity::onChildAdded(Node& child)
    {
        reinterpret_cast<Entity&>(child).setParent(this);
    }

    void Entity::onRemoveChild(Node& child)
    {
        reinterpret_cast<Entity&>(child).setParent(nullptr);
    }

    bool Entity::serializeTransform(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("position");
        std::string str = getPosition().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.Key("rotation");
        str = getRotation().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.Key("scale");
        str = getScale().string();
        writer.String(str.c_str(), static_cast<rapidjson::SizeType>(str.size()));

        writer.EndObject();

        return true;
    }

    bool Entity::deserializeTransform(const rapidjson::Value& jsonTransform)
    {
        if (!jsonTransform.IsObject())
        {
            DEBUG_LOG_ERROR("Unable to deserialize transform - Json value should be an object");
            return false;
        }

        auto it = jsonTransform.FindMember("position");
        if (it == jsonTransform.MemberEnd() || !it->value.IsString() || it->value.GetStringLength() == 0)
        {
            DEBUG_LOG_ERROR("Failed to deserialize transform - Invalid position string");
            return false;
        }

        Vector3 vec3;
        {
            std::stringstream str(it->value.GetString());
            str >> vec3;
            setPosition(vec3);
        }

        it = jsonTransform.FindMember("rotation");
        if (it == jsonTransform.MemberEnd() || !it->value.IsString() || it->value.GetStringLength() == 0)
        {
            DEBUG_LOG_ERROR("Failed to deserialize transform - Invalid rotation string");
            return false;
        }

        {
            Quaternion        rotation;
            std::stringstream str(it->value.GetString());
            str >> rotation;
            setRotation(rotation);
        }

        it = jsonTransform.FindMember("scale");
        if (it == jsonTransform.MemberEnd() || !it->value.IsString() || it->value.GetStringLength() == 0)
        {
            DEBUG_LOG_ERROR("Failed to deserialize transform - Invalid scale string");
            return false;
        }

        {
            std::stringstream str(it->value.GetString());
            str >> vec3;
            setScale(vec3);
        }

        return true;
    }

    bool Entity::serializeComponent(const ComponentPtr& component, rapidjson::Writer<rapidjson::StringBuffer>& writer)
    {
        writer.StartObject();

        const std::string typeName = component->getTypeName();

        writer.Key("type");
        writer.String(typeName.c_str(), static_cast<rapidjson::SizeType>(typeName.size()));

        writer.Key("data");
        if (!((IJsonSerializable&)*component).serialize(writer))
        {
            DEBUG_LOG_ERROR("Unable to serialize entity - Component serialization failed");
            return false;
        }

        writer.EndObject();

        return true;
    }

    bool Entity::addComponentFromJson(const rapidjson::Value& jsonComponent)
    {
        if (!jsonComponent.IsObject())
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity component - Json value should be an object");
            return false;
        }

        auto it = jsonComponent.FindMember("type");
        if (it == jsonComponent.MemberEnd() || !it->value.IsString())
        {
            DEBUG_LOG_ERROR("Failed to deserialize entity component - Invalid type string");
            return false;
        }

        const std::string type = it->value.GetString();

        it = jsonComponent.FindMember("data");
        if (it == jsonComponent.MemberEnd() || !it->value.IsObject())
        {
            DEBUG_LOG_ERROR("Failed to deserialize entity component - Invalid component data");
            return false;
        }

        const ComponentPtr component = Component::create(type, *this);
        if (!component || !((IJsonSerializable&)*component).deserialize(it->value))
            return false;

        m_components.push_back(component);
        return true;
    }

    bool Entity::serializeTransform(std::vector<char>& output) const
    {
        output.reserve(output.size() + sizeof(Vector3) * 2 + sizeof(Quaternion));

        if (!serializeVector3(getPosition(), output))
        {
            DEBUG_LOG_ERROR("Unable to serialize transform - Failed to write position");
            return false;
        }

        if (!serializeQuaternion(getRotation(), output))
        {
            DEBUG_LOG_ERROR("Unable to serialize transform - Failed to write rotation");
            return false;
        }

        if (!serializeVector3(getScale(), output))
        {
            DEBUG_LOG_ERROR("Unable to serialize transform - Failed to write scale");
            return false;
        }

        return true;
    }

    bool Entity::deserializeTransform(const char* data, const size_t length)
    {
        Vector3 tmp;
        if (!deserializeVector3(tmp, data, length))
        {
            DEBUG_LOG_ERROR("Unable to deserialize transform - Failed to read position");
            return false;
        }

        setPosition(tmp);
        size_t offset = sizeof(Vector3);

        Quaternion quat;
        if (!deserializeQuaternion(quat, data + offset, length - offset))
        {
            DEBUG_LOG_ERROR("Unable to deserialize transform - Failed to read rotation");
            return false;
        }

        setRotation(quat);
        offset += sizeof(Quaternion);

        if (!deserializeVector3(tmp, data + offset, length - offset))
        {
            DEBUG_LOG_ERROR("Unable to deserialize transform - Failed to read scale");
            return false;
        }

        setScale(tmp);
        return true;
    }

    bool Entity::serializeComponent(const ComponentPtr& component, std::vector<char>& output)
    {
        if (!serializeString<uint8_t>(component->getTypeName(), output))
        {
            DEBUG_LOG_ERROR("Unable to write entity to memory buffer - Failed to write component type string");
            return false;
        }

        if (!component->serializeWithSize(output))
        {
            DEBUG_LOG_ERROR("Unable to write entity to memory buffer - Failed to serialize component");
            return false;
        }

        return true;
    }

    size_t Entity::addComponentFromMemory(const char* data, const size_t length)
    {
        const size_t componentIdx = m_components.size();

        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity component [%d] - Invalid buffer", componentIdx);
            return 0;
        }

        std::string type;
        if (!deserializeString<uint8_t>(type, data, length))
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity component [%d] - Failed to read type string", componentIdx);
            return 0;
        }

        size_t offset = sizeof(uint8_t) + type.size();
        if (length <= offset || length - offset < sizeof(ElemSizeT))
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity component [%d] - Invalid offset", componentIdx);
            return 0;
        }

        const ElemSizeT elemSize = readSize(data + offset, length - offset);

        if (elemSize == INVALID_ELEMENT_SIZE)
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity component [%d] - Failed to read component's buffer size", componentIdx);
            return 0;
        }

        offset += sizeof(ElemSizeT);
        if (length <= offset || length - offset < elemSize)
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity component [%d] - Invalid offset", componentIdx);
            return 0;
        }

        const ComponentPtr component = Component::create(type, *this);
        if (!component || !((IByteSerializable&)*component).deserialize(data + offset, elemSize))
            return 0;

        m_components.push_back(component);
        return offset + elemSize;
    }

    size_t Entity::addChildFromMemory(const char* data, const size_t length)
    {
        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to deserialize child entity - Invalid buffer");
            return 0;
        }

        const ElemSizeT elemSize = readSize(data, length);

        if (elemSize == INVALID_ELEMENT_SIZE)
        {
            DEBUG_LOG_ERROR("Unable to deserialize child entity - Failed to read entity's buffer size");
            return 0;
        }

        if (length <= sizeof(ElemSizeT) || length - sizeof(ElemSizeT) < elemSize)
        {
            DEBUG_LOG_ERROR("Unable to deserialize child entity - Invalid offset");
            return 0;
        }

        Entity& entity = addChild<Entity>(this, Transform());
        if (!entity.deserialize(data + sizeof(ElemSizeT), elemSize))
        {
            DEBUG_LOG_ERROR("Unable to deserialize entity - Failed to deserialize child [%llu]", getChildren().size() - 1);
            return 0;
        }

        return sizeof(ElemSizeT) + elemSize;
    }
}
