#pragma once
#include "PantheonCore/Resources/ResourceRegistry.h"

namespace PantheonCore::Resources
{
    template <typename T, typename... Args>
    T* createResource(Args&&... args)
    {
        return new T(std::forward<Args>(args)...);
    }

    template <typename T>
    T* getDefaultResource()
    {
        return nullptr;
    }

    inline ResourceRegistry& ResourceRegistry::getInstance()
    {
        static ResourceRegistry instance;
        return instance;
    }

    inline IResource* ResourceRegistry::create(const std::string& type) const
    {
        return ASSUME(contains(type)) ? getTypeInfo(type).allocate() : nullptr;
    }

    inline IResource* ResourceRegistry::getDefault(const std::string& type) const
    {
        return ASSUME(contains(type)) ? getTypeInfo(type).getDefault() : nullptr;
    }

    template <typename T>
    void ResourceRegistry::registerType(const std::string& name)
    {
        const ResourceTypeInfo typeInfo
        {
            .allocate = []
            {
                return static_cast<IResource*>(createResource<T>());
            },
            .getDefault = []
            {
                return static_cast<IResource*>(getDefaultResource<T>());
            }
        };

        TypeRegistry::registerType<T>(name, typeInfo);
    }
}
