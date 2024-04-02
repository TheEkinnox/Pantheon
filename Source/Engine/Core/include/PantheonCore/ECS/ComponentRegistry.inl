#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentStorage.h"

#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"

namespace PantheonCore::ECS
{
    template <typename T>
    bool ComponentRegistry::toJson(
        const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, [[maybe_unused]] const EntitiesMap& toSerialized)
    {
        if constexpr (std::is_base_of_v<Serialization::IJsonSerializable, T>)
            return component.toJson(writer);
        else
            return ASSUME(false, "Json serialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    bool ComponentRegistry::fromJson(T& out, const rapidjson::Value& json)
    {
        if constexpr (std::is_base_of_v<Serialization::IJsonSerializable, T>)
            return out.fromJson(json);
        else
            return ASSUME(false, "Json deserialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    bool ComponentRegistry::toBinary(const T& component, std::vector<char>& out, [[maybe_unused]] const EntitiesMap& toSerialized)
    {
        if constexpr (std::is_base_of_v<Serialization::IByteSerializable, T>)
            return component.toBinary(out);
        else
            return ASSUME(false, "Binary serialization is not defined for \"%s\"", typeid(T).name()) && false;
    }

    template <typename T>
    size_t ComponentRegistry::fromBinary(T& out, const char* data, const size_t length)
    {
        if constexpr (std::is_base_of_v<Serialization::IByteSerializable, T>)
            return out.fromBinary(data, length);
        else
            return (ASSUME(false, "Binary deserialization is not defined for \"%s\"", typeid(T).name()), 0);
    }

    inline ComponentRegistry& ComponentRegistry::getInstance()
    {
        static ComponentRegistry instance;
        return instance;
    }

    template <typename T>
    void ComponentRegistry::registerType(const std::string& name)
    {
        const size_t typeHash = typeid(T).hash_code();

        const ComponentTypeInfo typeInfo
        {
            .m_typeId = typeHash,
            .makeStorage = [](Scene* scene)
            {
                std::unique_ptr<IComponentStorage> storage = std::make_unique<ComponentStorage<T>>(scene);
                return storage;
            }
        };

        TypeRegistry::registerType<T>(name, typeInfo);
    }
}
