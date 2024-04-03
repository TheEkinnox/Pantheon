#pragma once
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Resources/ResourceManager.h"
#include "PantheonCore/Resources/ResourceRef.h"

namespace PantheonCore::Resources
{
    template <typename T>
    ResourceRef<T> ResourceManager::load(const std::string& key, const std::string& path)
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        remove(key);
        removePath(path);

        if (key.empty() || path.empty())
            return {};

        T* resource = createResource<T>();

        if (!loadResource(resource, key, path))
        {
            m_resourceKeys.erase(path);
            return {};
        }

        m_resourceKeys[path] = key;

        return *(m_resources[key] = std::make_unique<ResourceRef<IResource>>(key, path, resource));
    }

    template <typename T>
    ResourceRef<T> ResourceManager::get(const std::string& keyOrPath) const
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        if (keyOrPath.empty())
            return {};

        auto it = m_resources.find(keyOrPath);

        if (it == m_resources.end())
        {
            const auto keyIt = m_resourceKeys.find(keyOrPath);

            if (keyIt == m_resourceKeys.end())
                return {};

            it = m_resources.find(keyIt->second);

            if (it == m_resources.end())
                return {};
        }

        return *it->second;
    }

    template <typename T>
    ResourceRef<T> ResourceManager::getOrCreate(const std::string& key, const std::string& path)
    {
        ResourceRef<T> resource = get<T>(key);

        if (!resource.hasValue())
            resource = get<T>(path);

        return resource.hasValue() ? resource : load<T>(key, path);
    }
}
