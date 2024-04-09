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

        if (key.empty() || path.empty())
            return {};

        const std::string savedPath = getResourcePath(key);

        if (savedPath != path)
        {
            const auto pathIt = m_resourceKeys.find(path);

            if (pathIt != m_resourceKeys.end() && !CHECK(m_resources[pathIt->second]->getReferenceCount() <= 1,
                    "Unsafe rebind of path \"%s\" from \"%s\" to \"%s\"", path.c_str(), pathIt->second.c_str(), key.c_str()))
                return {};
        }

        T*         resource = nullptr;
        const auto it       = m_resources.find(key);

        if (it != m_resources.end() && it->second)
        {
            IResource* savedResource = it->second->get();

            resource = dynamic_cast<T*>(savedResource);

            if (!CHECK(!savedResource || (resource && savedPath == path) || it->second->getReferenceCount() <= 1,
                    "Unsafe reloading of resource \"%s\" from \"%s\" to \"%s\"", key.c_str(), savedPath.c_str(), path.c_str()))
                return {};
        }

        const bool canReuse = resource;

        if (!resource)
            resource = createResource<T>();

        if (savedPath != path)
        {
            m_resourceKeys.erase(savedPath);
            removePath(path);
        }

        if (!loadResource(resource, key, path))
        {
            m_resources.erase(it);
            return {};
        }

        m_resourceKeys[path] = key;

        return canReuse ? *it->second : *(m_resources[key] = std::make_unique<ResourceRef<IResource>>(key, path, resource));
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

        if (!resource)
            resource = get<T>(path);

        return resource ? resource : load<T>(key, path);
    }
}
