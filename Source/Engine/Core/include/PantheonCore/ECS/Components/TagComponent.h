#pragma once
#include "PantheonCore/ECS/Component.h"

namespace PantheonCore::ECS
{
    class TagComponent : public Component
    {
        REGISTERED_COMPONENT_BODY(TagComponent)

    public:
        /**
         * \brief Creates a default tag component with the given owner
         * \param owner The component's owner
         */
        explicit TagComponent(Entity& owner);

        /**
         * \brief Creates a tag component with the given text and owner
         * \param owner The component's owner
         * \param tag The tag text
         */
        explicit TagComponent(Entity& owner, std::string tag);

        /**
         * \brief Creates a copy of the given tag component
         * \param other The tag component to copy
         */
        TagComponent(const TagComponent& other) = default;

        /**
         * \brief Creates a move copy of the given tag component
         * \param other The tag component to move
         */
        TagComponent(TagComponent&& other) noexcept = default;

        /**
         * \brief Serializes the tag component into the given byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the tag component from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

        /**
         * \brief Serializes the tag component to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the tag component from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool deserialize(const rapidjson::Value& json) override;

        /**
         * \brief Gets the tag component's text
         * \return The tag's text
         */
        std::string getTag() const;

        /**
         * \brief Sets the tag component's text
         * \param tag The tag component's new text
         */
        void setTag(const std::string& tag);

    private:
        std::string m_tag;

        /**
         * \brief The action to perform when the component is added
         */
        bool onAdd() override;
    };
}
