#include "PantheonCore/Resources/ResourceManager.h"
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Assets/AssetBundle.h"

#include <ranges>

#include "PantheonCore/Debug/Logger.h"

namespace PantheonEngine::Core::Resources
{
    ResourceManager::~ResourceManager()
    {
        clear();
    }

    ResourceManager& ResourceManager::operator=(const ResourceManager& other)
    {
        if (this == &other)
            return *this;

        m_resources = other.m_resources;

        return *this;
    }

    ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_resources = std::move(other.m_resources);

        return *this;
    }

    void ResourceManager::importBundle(const Assets::AssetBundle& bundle)
    {
        const auto assets = bundle.getAssets();

        if (assets.empty())
            return;

        for (const auto& asset : assets)
        {
            const char* type = asset.getType();
            const char* guid = asset.getGuid();

            IResource* ptr = create(type, guid);

            if (ptr == nullptr)
            {
                DEBUG_LOG("[WARNING] Skipped asset at path \"%s\" - Unable to create resource of type \"%s\"",
                    asset.getPath(), type);

                continue;
            }

            const std::vector<char> assetData = bundle.getAssetWithGuid(guid);

            if (assetData.empty())
            {
                DEBUG_LOG("[WARNING] Skipped asset at path \"%s\" - Empty data", asset.getPath());
                remove(guid);
                continue;
            }

            if (!ptr->deserialize(assetData.data(), assetData.size()))
            {
                DEBUG_LOG("[WARNING] Skipped asset at path \"%s\" - Unable to load resource", asset.getPath());
                remove(guid);
                continue;
            }

            m_resources[guid] = ptr;
        }
    }

    IResource* ResourceManager::create(const std::string& type, const std::string& key)
    {
        if (m_resources.contains(key))
        {
            delete m_resources[key];
            m_resources[key] = nullptr;
        }

        IResource* resource = IResource::create(type);

        if (resource == nullptr)
            return nullptr;

        m_resources[key] = resource;
        return resource;
    }

    void ResourceManager::remove(const std::string& key)
    {
        if (!m_resources.contains(key))
            return;

        delete m_resources[key];
        m_resources.erase(key);
    }

    void ResourceManager::clear()
    {
        for (const auto& resource : m_resources | std::views::values)
            delete resource;

        m_resources.clear();
    }
}
