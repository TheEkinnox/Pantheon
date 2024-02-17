#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"

#include <string>
#include <unordered_map>

#define REGISTER_RESOURCE_TYPE(Name, Type)                                                        \
static uint8_t resReg_##Name = (PantheonCore::Resources::IResource::registerType<Type>(#Name), 0);

#define REGISTERED_RESOURCE_BODY(Type)                                        \
public:                                                                       \
inline const std::string& Type::getTypeName() const                           \
{                                                                             \
    return PantheonCore::Resources::IResource::getRegisteredTypeName<Type>(); \
}                                                                             \
private:

namespace PantheonCore::Resources
{
    class IResource;

    template <typename T, typename... Args>
    T* createResource(Args&&... args);

    class IResource : public Serialization::IByteSerializable
    {
    public:
        /**
         * \brief Destroys the resource
         */
        ~IResource() override = default;

        /**
         * \brief Registers the given resource type (required for the create function)
         * \tparam T The Resource type to register
         * \param name The name associated to the registered type
         */
        template <typename T>
        static constexpr void registerType(const std::string& name);

        /**
         * \brief Gets the registered name for the given resource type
         * \tparam T The resource type
         * \return The registered name for the given resource type
         */
        template <typename T>
        static const std::string& getRegisteredTypeName();

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
        virtual const std::string& getTypeName() const = 0;

        /**
         * \brief Tries to load the resource from the given file
         * \param fileName The resource file's path
         * \return True if the resource was successfully loaded. False otherwise.
         */
        virtual bool load(const std::string& fileName) = 0;

        /**
         * \brief Initializes the resource
         * \return True on success. False otherwise.
         */
        virtual bool init() = 0;

    private:
        using AllocFunc = decltype(&createResource<IResource>);
        using TypeMap = std::unordered_map<std::string, AllocFunc>;
        using TypeNameMap = std::unordered_map<size_t, std::string>;

        inline static TypeMap     s_resourceTypes{};
        inline static TypeNameMap s_resourceTypeNames{};
    };
}

#include "IResource.inl"
