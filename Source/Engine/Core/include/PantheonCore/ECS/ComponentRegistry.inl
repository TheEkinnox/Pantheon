#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentStorage.h"
#include "PantheonCore/Serialization/EnumSerializer.h"

namespace PantheonCore::ECS
{
    template <typename T>
    bool ComponentRegistry::toJson(const T& in, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        constexpr bool hasToJsonWithMap = requires
        {
            in.toJson(writer, toSerialized);
        };

        if constexpr (hasToJsonWithMap)
            return in.toJson(writer, toSerialized);
        else
            return Serialization::toJson(in, writer);
    }

    template <typename T>
    bool ComponentRegistry::fromJson(T& out, const rapidjson::Value& json, Scene* scene)
    {
        constexpr bool hasFromJsonWithScene = requires
        {
            out.fromJson(json, scene);
        };

        if constexpr (hasFromJsonWithScene)
            return out.fromJson(json, scene);
        else
            return Serialization::fromJson(out, json);
    }

    template <typename T>
    bool ComponentRegistry::toBinary(const T& in, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        constexpr bool hasToBinaryWithMap = requires
        {
            in.toBinary(out, toSerialized);
        };

        if constexpr (hasToBinaryWithMap)
            return in.toBinary(out, toSerialized);
        else
            return Serialization::toBinary(in, out);
    }

    template <typename T>
    size_t ComponentRegistry::fromBinary(T& out, const char* data, const size_t length, Scene* scene)
    {
        constexpr bool hasFromBinaryWithScene = requires
        {
            out.fromBinary(data, length, scene);
        };

        if constexpr (hasFromBinaryWithScene)
            return out.fromBinary(data, length, scene);
        else
            return Serialization::fromBinary(out, data, length);
    }

    inline ComponentRegistry& ComponentRegistry::getInstance()
    {
        static ComponentRegistry instance;
        return instance;
    }

    template <typename T>
    void ComponentRegistry::registerType(const std::string& name)
    {
        const ComponentTypeInfo typeInfo
        {
            .m_typeId = ComponentRegistry::getTypeId<T>(),
            .makeStorage = [](Scene* scene)
            {
                std::shared_ptr<IComponentStorage> storage = std::make_shared<ComponentStorage<T>>(scene);
                return storage;
            }
        };

        TypeRegistry::registerType<T>(name, typeInfo);
    }
}
