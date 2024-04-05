#include "PantheonCore/Resources/ResourceManager.h"

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Resources/IResource.h"
#include "PantheonCore/Resources/ResourceRegistry.h"
#include "PantheonCore/Utility/FileSystem.h"

#include <ranges>

using namespace PantheonCore::Assets;

namespace PantheonCore::Resources
{
    ResourceManager::ResourceManager(std::vector<std::string> searchPaths)
        : m_searchPaths(std::move(searchPaths))
    {
    }

    ResourceManager::~ResourceManager()
    {
        clear();
    }

    ResourceManager& ResourceManager::operator=(const ResourceManager& other)
    {
        if (this == &other)
            return *this;

        m_bundles     = other.m_bundles;
        m_searchPaths = other.m_searchPaths;

        return *this;
    }

    ResourceManager& ResourceManager::operator=(ResourceManager&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_bundles      = std::move(other.m_bundles);
        m_resources    = std::move(other.m_resources);
        m_resourceKeys = std::move(other.m_resourceKeys);
        m_searchPaths  = std::move(other.m_searchPaths);

        return *this;
    }

    bool ResourceManager::includeBundle(const std::string& path, const bool shouldLoadResources)
    {
        if (!CHECK(!path.empty(), "Unable to include bundle - empty path"))
            return false;

        AssetBundle& assetBundle = m_bundles[path];

        if (!assetBundle.load(getFullPath(path)))
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

    GenericResourceRef ResourceManager::create(
        const std::string& type, const std::string& key, const std::string& path, const bool shouldLoad)
    {
        if (type.empty() || key.empty() || path.empty())
            return {};

        const std::string savedPath = getResourcePath(key);

        IResource* resource = nullptr;
        const auto it       = m_resources.find(key);

        if (it != m_resources.end() && it->second)
        {
            IResource* savedResource = it->second->get();

            resource = !savedResource || savedResource->getTypeName() == type ? savedResource : nullptr;

            if (!CHECK(!savedResource || (resource && savedPath == path) || it->second->getReferenceCount() <= 1,
                    "Unsafe reloading of resource \"%s\" from \"%s\" to \"%s\"", key.c_str(), savedPath.c_str(), path.c_str()))
                return {};
        }

        const bool canReuse = resource;

        if (!resource)
            resource = ResourceRegistry::getInstance().create(type);

        if (savedPath != path)
        {
            m_resourceKeys.erase(savedPath);
            removePath(path);
        }

        if (shouldLoad && !loadResource(resource, key, path))
        {
            m_resources.erase(it);
            return {};
        }

        m_resourceKeys[path] = key;

        return canReuse ? *it->second : *(m_resources[key] = std::make_unique<GenericResourceRef>(type, key, path, resource));
    }

    GenericResourceRef ResourceManager::get(const std::string& type, const std::string& keyOrPath) const
    {
        if (type.empty() || keyOrPath.empty())
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

        GenericResourceRef* resource = dynamic_cast<GenericResourceRef*>(it->second.get());

        if (!resource)
            return { *it->second, type };

        return resource->getType() == type ? *resource : GenericResourceRef();
    }

    GenericResourceRef ResourceManager::getOrCreate(const std::string& type, const std::string& key, const std::string& path)
    {
        GenericResourceRef resource(get(type, key));

        if (!resource)
            resource = get(type, path);

        return resource ? resource : create(type, key, path, true);
    }

    std::vector<char> ResourceManager::readFile(const std::string& keyOrPath) const
    {
        std::vector<char> resourceData;

        for (auto& bundle : m_bundles | std::views::values)
        {
            resourceData = bundle.getAssetWithGuid(keyOrPath);

            if (resourceData.empty())
                resourceData = bundle.getAssetAtPath(keyOrPath);

            if (resourceData.empty())
                continue;

            return resourceData;
        }

        const std::string path = getResourcePath(keyOrPath);
        std::ifstream     fileStream(getFullPath(!path.empty() ? path : keyOrPath), std::ios::binary | std::ios::ate);

        if (!fileStream.is_open())
            return {};

        const std::ifstream::pos_type length = fileStream.tellg();
        fileStream.seekg(0, std::ios::beg);

        resourceData.resize(length);
        fileStream.read(resourceData.data(), length);
        fileStream.close();

        return resourceData;
    }

    void ResourceManager::remove(const std::string& key)
    {
        m_resourceKeys.erase(getResourcePath(key));

        const auto it = m_resources.find(key);

        if (it == m_resources.end())
            return;

        m_resources.erase(it);
    }

    void ResourceManager::removePath(const std::string& path)
    {
        const auto it = m_resourceKeys.find(path);

        if (it == m_resourceKeys.end())
            return;

        m_resources.erase(it->second);
        m_resourceKeys.erase(it);
    }

    void ResourceManager::clear()
    {
        m_resources.clear();
        m_resourceKeys.clear();
    }

    std::vector<std::string> ResourceManager::getSearchPaths() const
    {
        return m_searchPaths;
    }

    void ResourceManager::setSearchPaths(std::vector<std::string> searchPaths)
    {
        m_searchPaths = std::move(searchPaths);
    }

    void ResourceManager::addSearchPath(const std::string& path)
    {
        const auto it = std::ranges::find(m_searchPaths, path);

        if (it == m_searchPaths.end())
            m_searchPaths.emplace_back(path);
    }

    void ResourceManager::removeSearchPath(const std::string& path)
    {
        m_searchPaths.erase(std::ranges::find(m_searchPaths, path));
    }

    std::string ResourceManager::getFullPath(const std::string& path) const
    {
        for (const auto& searchPath : m_searchPaths)
        {
            const std::string fullPath = Utility::appendPath(searchPath, path);

            if (Utility::pathExists(fullPath))
                return fullPath;
        }

        return path;
    }

    void ResourceManager::importBundle(const AssetBundle& bundle)
    {
        const auto assets = bundle.getAssets();

        if (assets.empty())
            return;

        for (const auto& asset : assets)
        {
            const char* type = asset->getType();
            const char* guid = asset->getGuid();
            const char* path = asset->getPath();

            GenericResourceRef resource(create(type, guid, path, false));

            if (!resource)
            {
                DEBUG_LOG("[WARNING] Skipped bundle asset at path \"%s\" - Unable to create resource of type \"%s\"", path, type);
                continue;
            }

            const std::vector<char> assetData = bundle.getAssetWithGuid(guid);

            if (assetData.empty())
            {
                DEBUG_LOG("[WARNING] Skipped bundle asset at path \"%s\" - Empty data", path);
                remove(guid);
                removePath(path);
                continue;
            }

            if (resource->fromBinary(assetData.data(), assetData.size()) == 0 || !resource->init())
            {
                DEBUG_LOG("[WARNING] Skipped bundle asset at path \"%s\" - Unable to load resource", path);
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

            return resource->fromBinary(bundleData.data(), bundleData.size()) != 0 && resource->init();
        }

        return resource->load(getFullPath(path)) && resource->init();
    }

    std::string ResourceManager::getResourcePath(const std::string& keyOrPath) const
    {
        const auto it = m_resourceKeys.find(keyOrPath);

        if (it != m_resourceKeys.end())
            return keyOrPath;

        for (const auto& [path, key] : m_resourceKeys)
        {
            if (key != keyOrPath)
                continue;

            return path;
        }

        for (const AssetBundle& bundle : m_bundles | std::views::values)
        {
            if (bundle.containsPath(keyOrPath))
                return keyOrPath;

            const char* path = bundle.getAssetPathFromGuid(keyOrPath);

            if (path != nullptr)
                return path;
        }

        return {};
    }
}
