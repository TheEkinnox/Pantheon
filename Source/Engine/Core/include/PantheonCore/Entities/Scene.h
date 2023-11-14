#pragma once
#include "PantheonCore/Entities/Entity.h"
#include "PantheonCore/DataStructure/Graph.h"
#include "PantheonCore/Resources/IResource.h"

namespace PantheonEngine::Core::Entities
{
    class Scene : public DataStructure::Graph<Entity>, public Resources::IResource, public Serialization::IJsonSerializable
    {
        REGISTERED_RESOURCE_BODY(Scene)

    public:
        virtual void update();

        /**
         * \brief Tries to load the resource from the given file
         * \param fileName The resource file's path
         * \return True if the resource was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        /**
         * \brief Serializes the scene to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const override;

        /**
         * \brief Deserializes the scene from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool deserialize(const rapidjson::Value& json) override;

        /**
         * \brief Serializes the object to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Tries to load the resource from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True if the resource was successfully loaded. False otherwise.
         */
        bool deserialize(const void* data, size_t length) override;
    };
}
