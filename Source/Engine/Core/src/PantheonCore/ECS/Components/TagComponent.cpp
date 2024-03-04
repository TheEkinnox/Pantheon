#include "PantheonCore/ECS/Components/TagComponent.h"

#include "PantheonCore/ECS/Entity.h"

namespace PantheonCore::ECS
{
    TagComponent::TagComponent(Entity& owner)
        : TagComponent(owner, "Entity")
    {
    }

    TagComponent::TagComponent(Entity& owner, std::string tag)
        : Component(owner), m_tag(std::move(tag))
    {
    }

    bool TagComponent::serialize(std::vector<char>& output) const
    {
        return serializeString(m_tag, output);
    }

    size_t TagComponent::deserialize(const void* data, size_t length)
    {
        return deserializeString(m_tag, static_cast<const char*>(data), length);
    }

    bool TagComponent::serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("tag");
        writer.String(m_tag.c_str(), static_cast<rapidjson::SizeType>(m_tag.size()));

        return writer.EndObject();
    }

    bool TagComponent::deserialize(const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize tag - Json value should be an object"))
            return false;

        const auto it = json.FindMember("tag");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize tag - Failed to read value"))
            return false;

        m_tag = it->value.GetString();
        return true;
    }

    std::string TagComponent::getTag() const
    {
        return m_tag;
    }

    void TagComponent::setTag(const std::string& tag)
    {
        m_tag = tag;
    }

    bool TagComponent::onAdd()
    {
        if (getOwner().getComponent<TagComponent>())
        {
            DEBUG_LOG("[WARNING] Entity already has a tag component");
            return false;
        }

        return Component::onAdd();
    }
}
