#include "PantheonCore/ECS/Scene.h"

#include "PantheonCore/ECS/ComponentRegistry.h"

#include <ranges>

#include <rapidjson/istreamwrapper.h>

namespace PantheonCore::ECS
{
    Scene::Scene()
        : m_entities(this)
    {
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

        return fromJson(json);
    }

    bool Scene::toBinary(std::vector<char>& output) const
    {
        if (!CHECK(writeNumber(m_entities.getCount(), output), "Unable to write scene entity count to memory buffer"))
            return false;

        IComponentStorage::EntitiesMap entitiesMap;
        Entity::Id                     index = 0;

        for (const auto entity : m_entities)
            entitiesMap[entity] = Entity(index++);

        const ElemCountT storageCount = static_cast<ElemCountT>(m_components.size());
        if (!CHECK(writeNumber(storageCount, output), "Unable to write scene entity count to memory buffer"))
            return false;

        for (const auto& [typeId, storage] : m_components)
        {
            if (!storage || storage->getCount() == 0)
                continue;

            const std::string& typeName = ComponentRegistry::getRegisteredTypeName(typeId);

            if (!CHECK(IByteSerializable::serializeString(typeName, output), "Unable to serialize component storage type string"))
                return false;

            if (!storage->toBinary(output, entitiesMap))
                return false;
        }

        return true;
    }

    size_t Scene::fromBinary(const char* data, const size_t length)
    {
        clear();

        if (data == nullptr || length == 0)
            return 0;

        Entity::Id entityCount = 0;
        size_t     offset      = readNumber(entityCount, data, length);

        if (!CHECK(offset != 0, "Unable to deserialize scene - Failed to read entity count"))
            return 0;

        m_entities.reserve(entityCount);

        for (Entity::Id id = 0; id < entityCount; ++id)
        {
            [[maybe_unused]] Entity entity = create();
            if (!ASSUME(entity.getIndex() == id))
                return 0;
        }

        ElemCountT storageCount = 0;
        size_t     readBytes    = length >= offset ? readNumber(storageCount, data + offset, length - offset) : 0;

        if (!CHECK(readBytes > 0, "Unable to deserialize scene - Failed to read storage count"))
            return 0;

        offset += readBytes;

        for (ElemCountT i = 0; i < storageCount; ++i)
        {
            readBytes = length >= offset ? deserializeStorage(data + offset, length - offset) : 0;

            if (readBytes == 0)
                return 0;

            offset += readBytes;
        }

        return offset;
    }

    bool Scene::toJson(rapidjson::Writer<rapidjson::StringBuffer>& writer) const
    {
        writer.StartObject();

        writer.Key("entities");
        writer.Uint64(m_entities.getCount());

        writer.Key("components");
        writer.StartArray();

        IComponentStorage::EntitiesMap entitiesMap;
        Entity::Id                     index = 0;

        for (const auto entity : m_entities)
            entitiesMap[entity] = Entity(index++);

        for (const auto& [typeId, storage] : m_components)
        {
            if (!storage || storage->getCount() == 0)
                continue;

            writer.StartObject();
            writer.Key("type");

            const std::string& typeName = ComponentRegistry::getRegisteredTypeName(typeId);
            if (!CHECK(writer.String(typeName.c_str(), static_cast<rapidjson::SizeType>(typeName.size())),
                    "Unable to serialize scene component storage - Failed to write type"))
                return false;

            writer.Key("data");
            if (!storage->toJson(writer, entitiesMap))
                return false;

            writer.EndObject();
        }

        writer.EndArray();

        return writer.EndObject();
    }

    bool Scene::fromJson(const rapidjson::Value& json)
    {
        clear();

        if (!CHECK(json.IsObject(), "Unable to deserialize scene - Json value should be an object"))
            return false;

        auto it = json.FindMember("entities");
        if (!CHECK(it != json.MemberEnd() && it->value.IsUint64(), "Unable to deserialize scene - Invalid entities count"))
            return false;

        const Entity::Id entityCount = it->value.GetUint64();
        m_entities.reserve(entityCount);

        for (Entity::Id id = 0; id < entityCount; ++id)
        {
            [[maybe_unused]] Entity entity = create();
            if (!ASSUME(entity.getIndex() == id))
                return false;
        }

        it = json.FindMember("components");
        if (!CHECK(it != json.MemberEnd() && it->value.IsArray(), "Unable to deserialize scene - Invalid components storage array"))
            return false;

        const auto componentStorages = it->value.GetArray();
        for (const auto& jsonStorage : componentStorages)
        {
            if (!deserializeStorage(jsonStorage))
                return false;
        }

        return true;
    }

    EntityHandle Scene::create()
    {
        return { this, m_entities.add() };
    }

    EntityHandle Scene::create(const Entity source)
    {
        if (!isValid(source))
            return create();

        Entity entity = m_entities.add();

        for (const auto& componentStorage : m_components | std::views::values)
            componentStorage->copy(source, entity);

        return { this, entity };
    }

    void Scene::destroy(const Entity entity)
    {
        for (const auto& componentStorage : m_components | std::views::values)
            componentStorage->remove(entity);

        m_entities.remove(entity);
    }

    bool Scene::isValid(const Entity entity) const
    {
        return entity != NULL_ENTITY && m_entities.has(entity);
    }

    void Scene::clear()
    {
        for (auto& storage : m_components | std::views::values)
        {
            if (storage)
                storage->clear();
        }

        m_entities.clear();
    }

    bool Scene::contains(const Entity owner) const
    {
        return m_entities.has(owner);
    }

    bool Scene::deserializeStorage(const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize scene component storage - Json value should be an object"))
            return false;

        auto it = json.FindMember("type");
        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize component storage - Invalid type string"))
            return false;

        const std::string type(it->value.GetString(), it->value.GetStringLength());

        it = json.FindMember("data");
        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize component storage - Data not found"))
            return false;

        const ComponentRegistry::TypeInfo& typeInfo = ComponentRegistry::getRegisteredTypeInfo(type);
        IComponentStorage&                 storage  = *(m_components[typeInfo.m_typeId] = typeInfo.makeStorage(this));

        if (!storage.fromJson(it->value))
            return false;

        return true;
    }

    size_t Scene::deserializeStorage(const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize component storage - Empty buffer"))
            return 0;

        std::string  typeName;
        const size_t offset = deserializeString(typeName, data, length);

        if (!CHECK(offset > 0, "Unable to deserialize component storage type string"))
            return 0;

        const ComponentRegistry::TypeInfo& typeInfo = ComponentRegistry::getRegisteredTypeInfo(typeName);
        IComponentStorage&                 storage  = *(m_components[typeInfo.m_typeId] = typeInfo.makeStorage(this));

        const size_t readBytes = length >= offset ? storage.fromBinary(data + offset, length - offset) : 0;

        if (readBytes == 0)
            return 0;

        return offset + readBytes;
    }
}
