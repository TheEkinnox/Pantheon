#pragma once
#include "PantheonCore/Utility/TypeRegistry.h"

#include <cstdint>

#define REGISTER_RESOURCE_TYPE(Name, Type)                                                                              \
static uint8_t resReg_##Name = (PantheonCore::Resources::ResourceRegistry::getInstance().registerType<Type>(#Name), 0);

namespace PantheonCore::Resources
{
    class IResource;

    template <typename T, typename... Args>
    T* createResource(Args&&... args);

    template <typename T>
    T* getDefaultResource();

    struct ResourceTypeInfo
    {
        decltype(&createResource<IResource>)     allocate;
        decltype(&getDefaultResource<IResource>) getDefault;
    };

    class ResourceRegistry final : public Utility::TypeRegistry<ResourceTypeInfo>
    {
    public:
        /**
         * \brief Gets the current resource registry instance
         * \return A reference to the current resource registry
         */
        static ResourceRegistry& getInstance();

        /**
         * \brief Registers the given component type with the given name
         * \tparam T The component type to register
         */
        template <typename T>
        void registerType(const std::string& name);

        /**
         * \brief Tries to allocate a resource of the given registered resource type.
         * \param type The type of the resource to create
         * \return A pointer to the allocated resource on success, nullptr otherwise
         */
        IResource* create(const std::string& type) const;

        /**
         * \brief Gets the default resource for the given registered resource type
         * \param type The target resource type
         * \return The default resource of the given type
         */
        IResource* getDefault(const std::string& type) const;
    };
}

#include "PantheonCore/Resources/ResourceRegistry.inl"
