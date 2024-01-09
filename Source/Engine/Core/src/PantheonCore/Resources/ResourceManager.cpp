#include "PantheonCore/Resources/ResourceManager.h"
#include "PantheonCore/Resources/IResource.h"

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

    bool ResourceManager::includeBundle(const std::string& path, const bool shouldLoadResources)
    {
        if (CHECK(!path.empty(), "Unable to include bundle - empty path"))
            return false;

        Assets::AssetBundle& assetBundle = m_bundles[path];

        if (!assetBundle.load(path))
        {
            m_bundles.erase(path);
            return false;
        }

        if (shouldLoadResources)
            importBundle(assetBundle);

        return true;
    }

    bool ResourceManager::removeBundle(const std::string& path)
    {
        const auto it = m_bundles.find(path);

        if (it == m_bundles.end())
            return false;

        const auto assets = it->second.getAssets();

        for (const auto& asset : assets)
        {
            remove(asset->getGuid());
            m_resourceKeys.erase(asset->getPath());
        }

        return true;
    }

    IResource* ResourceManager::create(const std::string& type, const std::string& key, const std::string& path,
                                       const bool         shouldLoad)
    {
        remove(key);
        removePath(path);

        IResource* resource = IResource::create(type);

        if (resource == nullptr || (shouldLoad && (!loadResource(resource, key, path))))
            return nullptr;

        m_resources[key] = resource;
        m_resourceKeys[path] = key;

        return resource;
    }

    IResource* ResourceManager::getOrCreate(const std::string& type, const std::string& key, const std::string& path)
    {
        IResource* resource = get<IResource>(key);

        if (!resource)
            resource = get<IResource>(path);

        return resource ? resource : create(type, key, path, true);
    }

    void ResourceManager::remove(const std::string& key)
    {
        const auto it = m_resources.find(key);

        if (it == m_resources.end())
            return;

        delete it->second;
        it->second = nullptr;
    }

    void ResourceManager::removePath(const std::string& path)
    {
        const auto it = m_resourceKeys.find(path);

        if (it == m_resourceKeys.end())
            return;

        remove(it->second);
        m_resourceKeys.erase(it);
    }

    void ResourceManager::clear()
    {
        for (const auto& resource : m_resources | std::views::values)
            delete resource;

        m_resources.clear();
    }

    void ResourceManager::importBundle(const Assets::AssetBundle& bundle)
    {
        const auto assets = bundle.getAssets();

        if (assets.empty())
            return;

        for (const auto& asset : assets)
        {
            const char* type = asset->getType();
            const char* guid = asset->getGuid();
            const char* path = asset->getPath();

            IResource* ptr = create(type, guid, path, false);

            if (ptr == nullptr)
            {
                DEBUG_LOG("[WARNING] Skipped asset at path \"%s\" - Unable to create resource of type \"%s\"", path, type);
                continue;
            }

            const std::vector<char> assetData = bundle.getAssetWithGuid(guid);

            if (assetData.empty())
            {
                DEBUG_LOG("[WARNING] Skipped asset at path \"%s\" - Empty data", path);
                remove(guid);
                removePath(path);
                continue;
            }

            if (!ptr->deserialize(assetData.data(), assetData.size()) || !ptr->init())
            {
                DEBUG_LOG("[WARNING] Skipped asset at path \"%s\" - Unable to load resource", path);
                remove(guid);
                removePath(path);
            }
        }
    }

    bool ResourceManager::loadResource(IResource* resource, const std::string& key, const std::string& path)
    {
        std::vector<char> bundleData;

        for (auto& bundle : m_bundles | std::views::values)
        {
            bundleData = bundle.getAssetWithGuid(key);

            if (bundleData.empty())
                bundleData = bundle.getAssetAtPath(path);

            if (bundleData.empty())
                continue;

            return resource->deserialize(bundleData.data(), bundleData.size()) && resource->init();
        }

        return resource->load(path) && resource->init();
    }
}
