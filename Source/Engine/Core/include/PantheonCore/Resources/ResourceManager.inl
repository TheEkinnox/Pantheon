#pragma once
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Resources/ResourceManager.h"

namespace PantheonEngine::Core::Resources
{
    template <typename T>
    T* ResourceManager::load(const std::string& key, const std::string& path)
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        if (m_resources.contains(key))
        {
            delete m_resources[key];
            m_resources[key] = nullptr;
        }

        T* ptr = new T();

        if (!ptr->load(path))
        {
            delete ptr;
            m_resourceKeys.erase(path);
            return nullptr;
        }

        m_resources[key] = ptr;
        m_resourceKeys[path] = key;

        return ptr;
    }

    template <typename T>
    T* ResourceManager::get(const std::string& keyOrPath) const
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        auto it = m_resources.find(keyOrPath);

        if (it == m_resources.end())
        {
            const auto keyIt = m_resourceKeys.find(keyOrPath);

            if (keyIt == m_resourceKeys.end())
                return nullptr;

            it = m_resources.find(keyIt->second);

            if (it == m_resources.end())
                return nullptr;
        }

        return reinterpret_cast<T*>(it->second);
    }

    template <typename T>
    T* ResourceManager::getOrCreate(const std::string& key, const std::string& path)
    {
        T* resource = get<T>(key);

        if (!resource)
            resource = get<T>(path);

        return resource ? resource : load<T>(key, path);
    }
}
