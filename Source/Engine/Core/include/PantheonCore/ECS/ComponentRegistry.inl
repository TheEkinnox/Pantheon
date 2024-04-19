#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentStorage.h"

#include "PantheonCore/Debug/Assertion.h"

namespace PantheonCore::ECS
{
    template <typename T>
    bool ComponentRegistry::toJson(const T& in, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized)
    {
        constexpr bool hasToJsonWithMap = requires
        {
            in.toJson(writer, toSerialized);
        };

        constexpr bool hasToJson = requires
        {
            in.toJson(writer);
        };

        if constexpr (hasToJsonWithMap)
            return in.toJson(writer, toSerialized);
        else if constexpr (hasToJson)
            return in.toJson(writer);
        else
            return ASSUME(false, "Json serialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    bool ComponentRegistry::fromJson(T& out, const rapidjson::Value& json, Scene* scene)
    {
        constexpr bool hasFromJsonWithScene = requires
        {
            out.fromJson(json, scene);
        };

        constexpr bool hasFromJson = requires
        {
            out.fromJson(json);
        };

        if constexpr (hasFromJsonWithScene)
            return out.fromJson(json, scene);
        else if constexpr (hasFromJson)
            return out.fromJson(json);
        else
            return ASSUME(false, "Json deserialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    bool ComponentRegistry::toBinary(const T& in, std::vector<char>& out, const EntitiesMap& toSerialized)
    {
        constexpr bool hasToBinaryWithMap = requires
        {
            in.toBinary(out, toSerialized);
        };

        constexpr bool hasToBinary = requires
        {
            in.toBinary(out);
        };

        if constexpr (hasToBinaryWithMap)
            return in.toBinary(out, toSerialized);
        else if constexpr (hasToBinary)
            return in.toBinary(out);
        else
            return ASSUME(false, "Binary serialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    size_t ComponentRegistry::fromBinary(T& out, const char* data, const size_t length, Scene* scene)
    {
        constexpr bool hasFromBinaryWithScene = requires
        {
            out.fromBinary(data, length, scene);
        };

        constexpr bool hasFromBinary = requires
        {
            out.fromBinary(data, length);
        };

        if constexpr (hasFromBinaryWithScene)
            return out.fromBinary(data, length, scene);
        else if constexpr (hasFromBinary)
            return out.fromBinary(data, length);
        else
            return ASSUME(false, "Binary deserialization is not defined for \"%s\"", typeid(T).name()) && false;
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
