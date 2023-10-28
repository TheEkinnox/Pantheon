#pragma once

#include <string>
#include <unordered_map>

#include "PantheonCore/Serialization/IByteSerializable.h"

#define REGISTER_RESOURCE_TYPE(Name, Type)                                                              \
static uint8_t reg_##Name = (PantheonEngine::Core::Resources::IResource::registerType<Type>(#Name), 0);

#define REGISTERED_RESOURCE_BODY(Type)                                                \
public:                                                                               \
inline std::string Type::getTypeName() const                                          \
{                                                                                     \
    return PantheonEngine::Core::Resources::IResource::getRegisteredTypeName<Type>(); \
}

namespace PantheonEngine::Core::Resources
{
    class IResource
    {
    public:
        /**
         * \brief Destroys the resource
         */
        virtual ~IResource() = default;

        /**
         * \brief Registers the given resource type (required for the create function)
         * \tparam T The Resource type to register
         */
        template <typename T>
        static constexpr void registerType(const std::string& name);

        /**
         * \brief Gets the registered name for the given resource type
         * \tparam T The resource type
         * \return The registered name for the given resource type
         */
        template <typename T>
        static std::string getRegisteredTypeName();

        /**
         * \brief Tries to allocate a resource of the given registered resource type.
         * \param type The type of the resource to create
         * \return A pointer to the allocated resource on success, nullptr otherwise
         */
        inline static IResource* create(const std::string& type);

        /**
         * \brief Gets the resource's registered type name
         * \return The resource's registered type name
         */
        virtual std::string getTypeName() const = 0;

        /**
         * \brief Tries to load the resource from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return True if the resource was successfully loaded. False otherwise.
         */
        virtual bool load(const void* data, size_t length) = 0;

        /**
         * \brief Tries to load the resource from the given file
         * \param fileName The resource file's path
         * \return True if the resource was successfully loaded. False otherwise.
         */
        virtual bool load(const std::string& fileName) = 0;


    private:
        using AllocFunc = IResource* (*)();
        using TypeMap = std::unordered_map<std::string, AllocFunc>;
        using TypeNameMap = std::unordered_map<size_t, std::string>;

        inline static TypeMap     s_resourceTypes{};
        inline static TypeNameMap s_resourceTypeNames{};
    };
}

#include "IResource.inl"
