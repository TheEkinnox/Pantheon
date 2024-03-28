#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/ComponentStorage.h"

#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"

namespace PantheonCore::ECS
{
    template <typename T>
    bool ComponentRegistry::toJson(
        const T* component, rapidjson::Writer<rapidjson::StringBuffer>& writer, [[maybe_unused]] const EntitiesMap& toSerialized)
    {
        if constexpr (std::is_base_of_v<Serialization::IJsonSerializable, T>)
        {
            return component && component->toJson(writer);
        }
        else
        {
            ((void)ASSUME(false, "Json serialization is not defined for \"%s\"", typeid(T).name()));
            return false;
        }
    }

    template <typename T>
    bool ComponentRegistry::fromJson(T* out, const rapidjson::Value& json)
    {
        if constexpr (std::is_base_of_v<Serialization::IJsonSerializable, T>)
        {
            return out && out->fromJson(json);
        }
        else
        {
            ((void)ASSUME(false, "Json deserialization is not defined for \"%s\"", typeid(T).name()));
            return false;
        }
    }

    template <typename T>
    bool ComponentRegistry::toBinary(const T* component, std::vector<char>& out, [[maybe_unused]] const EntitiesMap& toSerialized)
    {
        if constexpr (std::is_base_of_v<Serialization::IByteSerializable, T>)
        {
            return component && component->toBinary(out);
        }
        else
        {
            ((void)ASSUME(false, "Binary serialization is not defined for \"%s\"", typeid(T).name()));
            return false;
        }
    }

    template <typename T>
    size_t ComponentRegistry::fromBinary(T* out, const char* data, const size_t length)
    {
        if constexpr (std::is_base_of_v<Serialization::IByteSerializable, T>)
        {
            return out ? const_cast<std::remove_const_t<T>*>(out)->fromBinary(data, length) : 0;
        }
        else
        {
            ((void)ASSUME(false, "Binary deserialization is not defined for \"%s\"", typeid(T).name()));
            return 0;
        }
    }

    template <typename T>
    void ComponentRegistry::registerType(const std::string& name)
    {
        const size_t typeHash = typeid(T).hash_code();
        ASSERT(!s_typeInfos.contains(typeHash), "Type %d (\"%s\") has already been registered", typeHash, typeid(T).name());
        ASSERT(!s_typeIds.contains(name), "Type \"%s\" has already been registered", name.c_str());

        const TypeInfo typeInfo
        {
            .m_name = name,
            .m_typeId = typeHash,
            .makeStorage = [](Scene* scene)
            {
                std::unique_ptr<IComponentStorage> storage = std::make_unique<ComponentStorage<T>>(scene);
                return storage;
            }
        };

        s_typeInfos[typeHash] = typeInfo;

        s_typeIds[name] = typeHash;
    }

    inline const ComponentRegistry::TypeInfo& ComponentRegistry::getRegisteredTypeInfo(const std::string& type)
    {
        const auto it = s_typeIds.find(type);
        ASSERT(it != s_typeIds.end(), "No registered type \"%s\" found.", type.c_str());
        return getRegisteredTypeInfo(it->second);
    }

    inline const ComponentRegistry::TypeInfo& ComponentRegistry::getRegisteredTypeInfo(size_t typeId)
    {
        const auto it = s_typeInfos.find(typeId);
        ASSERT(it != s_typeInfos.end(), "No registered type id \"%d\" found.", typeId);
        return it->second;
    }

    inline const std::string& ComponentRegistry::getRegisteredTypeName(size_t typeId)
    {
        const auto it = s_typeInfos.find(typeId);
        ASSERT(it != s_typeInfos.end(), "Couldn't find registered name of type with id %d", typeId);

        return it->second.m_name;
    }

    template <typename T>
    const ComponentRegistry::TypeInfo& ComponentRegistry::getRegisteredTypeInfo()
    {
        return getRegisteredTypeInfo(typeid(T).hash_code());
    }

    template <typename T>
    const std::string& ComponentRegistry::getRegisteredTypeName()
    {
        return getRegisteredTypeName(typeid(T).hash_code());
    }
}
