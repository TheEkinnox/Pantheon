#pragma once
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/Utility/DynamicTypeInfo.h"
#include "PantheonCore/Serialization/IJsonSerializable.h"
#include "PantheonCore/Utility/TypeRegistry.h"

#include <cstdint>
#include <memory>

#define REGISTER_COMPONENT_TYPE(Name, Type)                                                                        \
static uint8_t compReg_##Name = (PantheonCore::ECS::ComponentRegistry::getInstance().registerType<Type>(#Name), 0);

namespace PantheonCore::ECS
{
    class IComponentStorage;
    class Scene;

    struct ComponentTypeInfo
    {
        using TypeId = size_t;

        TypeId m_typeId;

        std::unique_ptr<IComponentStorage> (*makeStorage)(Scene*);
    };

    class ComponentRegistry final : public Utility::TypeRegistry<Utility::DynamicTypeInfo<ComponentTypeInfo>>
    {
    public:
        using EntitiesMap = std::unordered_map<Entity::Id, Entity>;

        template <typename T>
        static bool toJson(const T& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized);

        template <typename T>
        static bool fromJson(T& out, const rapidjson::Value& json);

        template <typename T>
        static bool toBinary(const T& component, std::vector<char>& out, const EntitiesMap& toSerialized);

        template <typename T>
        static size_t fromBinary(T& out, const char* data, size_t length);

        /**
         * \brief Gets the current component registry instance
         * \return A reference to the current component registry
         */
        static ComponentRegistry& getInstance();

        /**
         * \brief Registers the given component type with the given name
         * \tparam T The component type to register
         */
        template <typename T>
        void registerType(const std::string& name);
    };
}

#include "PantheonCore/ECS/ComponentRegistry.inl"
