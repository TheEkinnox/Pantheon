#pragma once
#include "PantheonCore/Utility/CoreDefines.h"
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/Serialization/Serializer.h"
#include "PantheonCore/Utility/TypeRegistry.h"

#if USING(PTH_TARGET_EDITOR)
#include "PantheonCore/Utility/DynamicTypeInfo.h"
#endif

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
        Utility::TypeId m_typeId;

        std::shared_ptr<IComponentStorage> (*makeStorage)(Scene*);
    };

#if USING(PTH_TARGET_EDITOR)
    class ComponentRegistry final : public Utility::TypeRegistry<Utility::DynamicTypeInfo<ComponentTypeInfo>>
#else
    class ComponentRegistry final : public Utility::TypeRegistry<ComponentTypeInfo>
#endif
    {
    public:
        using EntitiesMap = std::unordered_map<Entity::Id, Entity>;

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

        template <typename T>
        static bool toBinary(const T& value, std::vector<char>& out, const EntitiesMap& toSerialized);

        template <typename T>
        static size_t fromBinary(T& out, const char* data, size_t length, Scene* scene);

        template <typename T>
        static bool toJson(const T& value, Serialization::JsonWriter& writer, const EntitiesMap& toSerialized);

        template <typename T>
        static bool fromJson(T& out, const Serialization::JsonValue& json, Scene* scene);
    };
}

#include "PantheonCore/ECS/ComponentRegistry.inl"
