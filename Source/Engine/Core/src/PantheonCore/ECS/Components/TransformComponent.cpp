#include "PantheonCore/ECS/Components/TransformComponent.h"

#include "PantheonCore/ECS/Entity.h"

using namespace LibMath;

namespace PantheonCore::ECS
{
    TransformComponent::TransformComponent(Entity& owner)
        : TransformComponent(owner, {})
    {
    }

    TransformComponent::TransformComponent(Entity& owner, const Transform& transform)
        : Component(owner), Transform(transform)
    {
        bindEvents();
    }

    TransformComponent::TransformComponent(Entity& owner, Transform&& transform)
        : Component(owner), Transform(std::move(transform))
    {
        bindEvents();
    }

    TransformComponent::TransformComponent(const TransformComponent& other)
        : Component(other), Transform(other)
    {
        bindEvents();
    }

    TransformComponent::TransformComponent(TransformComponent&& other) noexcept
        : Component(std::forward<Component&&>(other)), Transform(std::move(other))
    {
        bindEvents();
    }

    TransformComponent::~TransformComponent()
    {
        unbindEvents();
    }

    TransformComponent& TransformComponent::operator=(const TransformComponent& other)
    {
        if (&other == this)
            return *this;

        unbindEvents();

        Component::operator=(other);
        Transform::operator=(other);

        bindEvents();

        return *this;
    }

    TransformComponent& TransformComponent::operator=(TransformComponent&& other) noexcept
    {
        if (&other == this)
            return *this;

        unbindEvents();

        Component::operator=(std::forward<Component&&>(other));
        Transform::operator=(std::move(other));

        bindEvents();

        return *this;
    }

    bool TransformComponent::serialize(std::vector<char>& output) const
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

    size_t TransformComponent::deserialize(const void* data, size_t length)
    {
        Vector3     tmp;
        const char* buffer = static_cast<const char*>(data);

        if (!CHECK(deserializeVector3(tmp, buffer, length), "Unable to deserialize transform position"))
            return 0;

        setPosition(tmp);
        size_t offset = sizeof(Vector3);

        Quaternion quat;
        if (!CHECK(deserializeQuaternion(quat, buffer + offset, length - offset), "Unable to deserialize transform rotation"))
            return 0;

        setRotation(quat);
        offset += sizeof(Quaternion);

        if (!CHECK(deserializeVector3(tmp, buffer + offset, length - offset), "Unable to deserialize transform scale"))
            return 0;

        setScale(tmp);
        return offset + sizeof(Vector3);
    }

    bool TransformComponent::serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
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

        return writer.EndObject();
    }

    bool TransformComponent::deserialize(const rapidjson::Value& json)
    {
        if (!json.IsObject())
        {
            DEBUG_LOG_ERROR("Unable to deserialize transform - Json value should be an object");
            return false;
        }

        auto it = json.FindMember("position");
        if (it == json.MemberEnd() || !it->value.IsString() || it->value.GetStringLength() == 0)
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

        it = json.FindMember("rotation");
        if (it == json.MemberEnd() || !it->value.IsString() || it->value.GetStringLength() == 0)
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

        it = json.FindMember("scale");
        if (it == json.MemberEnd() || !it->value.IsString() || it->value.GetStringLength() == 0)
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

    void TransformComponent::bindEvents()
    {
        Entity& owner = getOwner();

        if (m_childAddedListenerId == 0)
        {
            const auto onChildAddedFunc = [this](Entity& child)
            {
                if (TransformComponent* transform = child.getComponentInChildren<TransformComponent>(true))
                    transform->setParent(this, false);
            };

            m_childAddedListenerId = owner.m_childAddedEvent.subscribe(onChildAddedFunc);
        }

        if (m_childRemovedListenerId == 0)
        {
            const auto onChildRemovedFunc = [this](Entity& child)
            {
                if (TransformComponent* transform = child.getComponent<TransformComponent>())
                    transform->setParent(nullptr, true);
            };

            m_childRemovedListenerId = owner.m_childRemovedEvent.subscribe(onChildRemovedFunc);
        }
    }

    void TransformComponent::unbindEvents()
    {
        if (m_childAddedListenerId != 0)
        {
            m_childAddedListenerId = 0;
            getOwner().m_childAddedEvent.unsubscribe(m_childAddedListenerId);
        }

        if (m_childRemovedListenerId != 0)
        {
            getOwner().m_childRemovedEvent.unsubscribe(m_childRemovedListenerId);
            m_childRemovedListenerId = 0;
        }
    }

    bool TransformComponent::onAdd()
    {
        if (getOwner().getComponent<TransformComponent>())
        {
            DEBUG_LOG("[WARNING] Entity already has a transform component");
            return false;
        }

        for (const auto& child : getOwner().getChildren())
        {
            if (TransformComponent* transform = reinterpret_cast<Entity&>(*child).getComponentInChildren<TransformComponent>(true))
                transform->setParent(this, false);
        }

        if (TransformComponent* transform = getOwner().getComponentInParent<TransformComponent>(true))
            setParent(transform, false);

        return Component::onAdd();
    }

    void TransformComponent::onChange()
    {
        Transform::onChange();
        m_changedEvent.invoke();
    }

    void TransformComponent::onRemove()
    {
        unbindEvents();
        Component::onRemove();
    }
}
