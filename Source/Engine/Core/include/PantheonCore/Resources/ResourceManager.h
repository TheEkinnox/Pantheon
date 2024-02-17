#pragma once
#include "PantheonCore/Assets/AssetBundle.h"

#include <string>
#include <unordered_map>

namespace PantheonCore::Resources
{
    class IResource;

    class ResourceManager
    {
        using ResourcePtr = IResource*;
        using ResourceMap = std::unordered_map<std::string, ResourcePtr>;
        using KeyMap = std::unordered_map<std::string, std::string>;
        using BundlesMap = std::unordered_map<std::string, Assets::AssetBundle>;

    public:
        /**
         * \brief Creates a default resource manager
         */
        ResourceManager() = default;

        /**
         * \brief Creates a copy of the given resource manager
         * \param other A reference to the resource manager to copy
         */
        ResourceManager(const ResourceManager& other) = default;

        /**
         * \brief Creates a move copy of the given resource manager
         * \param other The resource manager to move
         */
        ResourceManager(ResourceManager&& other) noexcept = default;

        /**
         * \brief Destroys the resource manager and frees the memory allocated for it's loaded resources
         */
        ~ResourceManager();

        /**
         * \brief Copies the given resource manager's data into the current one
         * \param other A reference to the resource manager to copy
         */
        ResourceManager& operator=(const ResourceManager& other);

        /**
         * \brief Moves the given resource manager's data into the current one
         * \param other The resource manager to move
         */
        ResourceManager& operator=(ResourceManager&& other) noexcept;

        /**
         * \brief Adds the asset bundle at the given path to the manager
         * \param path The bundle's file path
         * \param shouldLoadResources Whether the bundle's assets should be loaded
         * \return True on success. False otherwise.
         */
        bool includeBundle(const std::string& path, bool shouldLoadResources = true);

        /**
         * \brief Removes the bundle and all it's resources with the given path from the manager
         * \param path The bundle's path
         * \return True if the bundle was removed. False otherwise
         */
        bool removeBundle(const std::string& path);

        /**
         * \brief Tries to load the resource with the given path
         * and stores it in the resource map with the given key.
         * \tparam T The resource's type
         * \param key The resource's key in the resource map
         * \param path The path of the resource's file
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* load(const std::string& key, const std::string& path);

        /**
         * \brief Tries to create an empty resource of the given type
         * and stores it in the resource map with the given key.
         * \param type The resource's type
         * \param key The resource's key
         * \param path The resource's path
         * \param shouldLoad Whether the resource should be loaded on creation
         * \return A pointer to the created resource on success, nullptr otherwise.
         */
        IResource* create(const std::string& type, const std::string& key, const std::string& path, bool shouldLoad = true);

        /**
         * \brief Tries to find the resource with the given key.
         * \tparam T The resource's type
         * \param keyOrPath The resource's key
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* get(const std::string& keyOrPath) const;

        /**
         * \brief Tries to find the resource with the given key or path. If not found, tries to load it from the given path
         * \tparam T The resource's type
         * \param key The resource's key
         * \param path The resource's path
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* getOrCreate(const std::string& key, const std::string& path);

        /**
         * \brief Tries to find the resource with the given key or path. If not found, tries to load it from the given path
         * \param type The resource's type
         * \param key The resource's key
         * \param path The resource's path
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        IResource* getOrCreate(const std::string& type, const std::string& key, const std::string& path);

        /**
         * \brief Reads the content of the resource at the given path
         * \param keyOrPath The key or path of the resource file to read
         * \return The resource's file content
         */
        std::vector<char> readFile(const std::string& keyOrPath) const;

        /**
         * \brief Removes the resource with the given key from the manager
         * \param key The resource's key
         */
        void remove(const std::string& key);

        /**
         * \brief Removes the resource with the given path from the manager
         * \param path The resource's path
         */
        void removePath(const std::string& path);

        /**
         * \brief Removes all loaded resources from the manager
         */
        void clear();

    private:
        BundlesMap  m_bundles;
        ResourceMap m_resources;
        KeyMap      m_resourceKeys;

        /**
         * \brief Loads all the resources contained in the given asset bundle
         * \param bundle The asset bundle from which resources should be loaded
         */
        void importBundle(const Assets::AssetBundle& bundle);

        /**
         * \brief Loads and initializes the given resource
         * \param resource The resource to load
         * \param key The resource's key
         * \param path The resource's path
         * \return True on success. False otherwise.
         */
        bool loadResource(IResource* resource, const std::string& key, const std::string& path);

        /**
         * \brief Gets the path of a resource from its key or path
         * \param keyOrPath The resource's key or path
         * \return The resource's path
         */
        std::string getResourcePath(const std::string& keyOrPath) const;
    };
}

#include "PantheonCore/Resources/ResourceManager.inl"
