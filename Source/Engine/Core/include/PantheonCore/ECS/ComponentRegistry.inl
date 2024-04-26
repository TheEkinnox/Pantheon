#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentStorage.h"

namespace PantheonCore::ECS
{
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

    template <typename T>
    bool ComponentRegistry::toBinary(const T& value, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        constexpr bool hasToBinaryWithMap = requires
        {
            value.toBinary(out, toSerialized);
        };

        if constexpr (hasToBinaryWithMap)
            return value.toBinary(out, toSerialized);
        else
            return Serialization::toBinary(value, out);
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

    template <typename T>
    bool ComponentRegistry::toJson(const T& value, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized)
    {
        constexpr bool hasToJsonWithMap = requires
        {
            value.toJson(writer, toSerialized);
        };

        if constexpr (hasToJsonWithMap)
            return value.toJson(writer, toSerialized);
        else
            return Serialization::toJson(value, writer);
    }

    template <typename T>
    bool ComponentRegistry::fromJson(T& out, const Serialization::JsonValue& json, Scene* scene)
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
}
