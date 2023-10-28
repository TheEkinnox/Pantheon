#pragma once

#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Resources/ResourceManager.h"

namespace PantheonEngine::Core::Resources
{
    template <typename T>
    T* ResourceManager::load(const std::string& key, const std::string& resourcePath)
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        if (m_resources.contains(key))
        {
            delete m_resources[key];
            m_resources[key] = nullptr;
        }

        T* ptr = new T();

        if (!ptr->load(resourcePath))
        {
            delete ptr;
            return nullptr;
        }

        m_resources[key] = ptr;

        return ptr;
    }

    template <typename T>
    T* ResourceManager::load(const std::string& key, const void* data, const size_t length)
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        if (m_resources.contains(key))
        {
            delete m_resources[key];
            m_resources[key] = nullptr;
        }

        T* ptr = new T();

        if (!ptr->deserialize(data, length))
        {
            delete ptr;
            return nullptr;
        }

        m_resources[key] = ptr;

        return ptr;
    }

    template <typename T>
    T* ResourceManager::get(const std::string& key) const
    {
        static_assert(std::is_same_v<IResource, T> || std::is_base_of_v<IResource, T>);

        if (!m_resources.contains(key))
            return nullptr;

        return reinterpret_cast<T*>(m_resources.at(key));
    }
}
