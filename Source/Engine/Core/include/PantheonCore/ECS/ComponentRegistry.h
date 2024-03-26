#pragma once
#include "PantheonCore/ECS/Entity.h"
#include "PantheonCore/Serialization/IJsonSerializable.h"

#include <cstdint>
#include <memory>
#include <string>
#include <unordered_map>

#define REGISTER_COMPONENT_TYPE(Name, Type)                                                       \
static uint8_t compReg_##Name = (PantheonCore::ECS::ComponentRegistry::registerType<Type>(#Name), 0);

namespace PantheonCore::ECS
{
    class IComponentStorage;
    class Scene;

    class ComponentRegistry final
    {
    public:
        using EntitiesMap = std::unordered_map<Entity::Id, Entity>;

        template <typename T>
        static bool toJson(const T* component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap& toSerialized);

        template <typename T>
        static bool fromJson(T* out, const rapidjson::Value& json);

        template <typename T>
        static bool toBinary(const T* component, std::vector<char>& out, const EntitiesMap& toSerialized);

        template <typename T>
        static size_t fromBinary(T* out, const char* data, size_t length);

        struct TypeInfo
        {
            using TypeId = size_t;

            std::string m_name;
            TypeId      m_typeId;

            std::unique_ptr<IComponentStorage> (*makeStorage)(Scene*);
        };

        ComponentRegistry()                                   = delete;
        ComponentRegistry(const ComponentRegistry& other)     = delete;
        ComponentRegistry(ComponentRegistry&& other) noexcept = delete;
        ~ComponentRegistry()                                  = default;

        ComponentRegistry& operator=(const ComponentRegistry& other)     = delete;
        ComponentRegistry& operator=(ComponentRegistry&& other) noexcept = delete;

        bool operator==(const ComponentRegistry& other) const;
        bool operator!=(const ComponentRegistry& other) const;

        /**
         * \brief Registers the given component type (required for the create function)
         * \tparam T The component type to register
         */
        template <typename T>
        static void registerType(const std::string& name);

        /**
         * \brief Gets the registered type information for the given component type
         * \tparam T The component type
         * \return The registered type information for the given component type
         */
        static const TypeInfo& getRegisteredTypeInfo(const std::string& type);

        /**
         * \brief Gets the registered type information for the given type id
         * \param typeId The component type's id
         * \return The registered type information for the given component type
         */
        static const TypeInfo& getRegisteredTypeInfo(size_t typeId);

        /**
         * \brief Gets the registered type information for the given type id
         * \tparam T The component type
         * \return The registered type information for the given component type
         */
        template <typename T>
        static const TypeInfo& getRegisteredTypeInfo();

        /**
         * \brief Gets the registered name for the given type id
         * \param typeId The component type's id
         * \return The registered name for the given component type
         */
        static const std::string& getRegisteredTypeName(size_t typeId);

        /**
         * \brief Gets the registered name for the given component type
         * \tparam T The component type
         * \return The registered name for the given component type
         */
        template <typename T>
        static const std::string& getRegisteredTypeName();

    private:
        using TypeMap = std::unordered_map<size_t, TypeInfo>;
        using TypeIdMap = std::unordered_map<std::string, size_t>;

        inline static TypeMap   s_typeInfos{};
        inline static TypeIdMap s_typeIds{};
    };
}

#include "PantheonCore/ECS/ComponentRegistry.inl"
