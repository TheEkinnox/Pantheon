#include "PantheonCore/Entities/Scene.h"

#include <rapidjson/istreamwrapper.h>

#include "PantheonCore/Utility/ByteOrder.h"

namespace PantheonCore::Entities
{
    void Scene::update()
    {
        for (const auto& node : getNodes())
            node->update();
    }

    bool Scene::load(const std::string& fileName)
    {
        std::ifstream fs(fileName);

        if (!fs.is_open())
        {
            DEBUG_LOG_ERROR("Unable to open scene file at path \"%s\"", fileName.c_str());
            return false;
        }

        rapidjson::IStreamWrapper isw(fs);

        rapidjson::Document json;
        json.ParseStream(isw);

        if (json.HasParseError())
        {
            DEBUG_LOG_ERROR("Unable to parse scene from file - Parse error %d", json.GetParseError());
            return false;
        }

        return deserialize(json);
    }

    bool Scene::serialize(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("entities");
        writer.StartArray();

        for (const auto& entity : getNodes())
        {
            if (!entity->serialize(writer))
            {
                DEBUG_LOG_ERROR("Unable to write scene to json - Scene entity serializing failed");
                return false;
            }
        }

        writer.EndArray();

        writer.EndObject();

        return true;
    }

    bool Scene::deserialize(const rapidjson::Value& json)
    {
        clear();

        if (!json.IsObject())
        {
            DEBUG_LOG_ERROR("Unable to deserialize scene - Json value should be an object");
            return false;
        }

        const auto it = json.FindMember("entities");
        if (it == json.MemberEnd() || !it->value.IsArray())
        {
            DEBUG_LOG_ERROR("Unable to deserialize scene - Invalid entities array");
            return false;
        }

        const auto array = it->value.GetArray();
        for (const auto& jsonEntity : array)
        {
            Entity& entity = addNode<Entity>();
            if (!entity.deserialize(jsonEntity))
            {
                DEBUG_LOG_ERROR("Unable to deserialize scene - Scene entity deserializing failed");
                return false;
            }
        }

        return true;
    }

    bool Scene::serialize(std::vector<char>& output) const
    {
        const ElemCountT entityCount = static_cast<ElemCountT>(getNodes().size());

        const size_t startSize = output.size();
        output.resize(startSize + sizeof(ElemCountT));

        const ElemCountT leEntityCount = Utility::toBigEndian(entityCount);
        if (memcpy_s(output.data() + startSize, output.size() - startSize, &leEntityCount, sizeof(ElemCountT)) != 0)
        {
            DEBUG_LOG_ERROR("Unable to write scene entity count to memory buffer");
            return false;
        }

        for (const std::shared_ptr<const Entity>& entity : getNodes())
        {
            if (!entity->serializeWithSize(output))
            {
                DEBUG_LOG_ERROR("Unable to write scene to memory buffer - Failed to serialize entity");
                return false;
            }
        }

        return true;
    }

    bool Scene::deserialize(const void* data, const size_t length)
    {
        clear();

        if (data == nullptr || length == 0)
            return false;

        ElemCountT entityCount = 0;

        if (length < sizeof(ElemCountT) || memcpy_s(&entityCount, sizeof(ElemCountT), data, sizeof(ElemCountT)))
        {
            DEBUG_LOG_ERROR("Unable to deserialize scene - Failed to read entity count");
            return false;
        }

        size_t      offset = sizeof(ElemCountT);
        const char* byteData = static_cast<const char*>(data);

        for (ElemCountT i = 0; i < Utility::fromBigEndian(entityCount); ++i)
        {
            if (length <= offset || length - offset < sizeof(ElemSizeT))
            {
                DEBUG_LOG_ERROR("Unable to deserialize scene - Invalid offset");
                return false;
            }

            const ElemSizeT bufferSize = readSize(byteData + offset, length - offset);

            if (bufferSize == INVALID_ELEMENT_SIZE)
            {
                DEBUG_LOG_ERROR("Unable to deserialize scene - Failed to read size of entity [%d]", i);
                return false;
            }

            offset += sizeof(ElemSizeT);
            Entity& entity = addNode<Entity>();

            if (length <= offset || length - offset < bufferSize || !entity.deserialize(byteData + offset, bufferSize))
            {
                DEBUG_LOG_ERROR("Unable to deserialize scene - Failed to deserialize entity [%d]", i);
                return false;
            }

            offset += bufferSize;
        }

        return true;
    }
}
