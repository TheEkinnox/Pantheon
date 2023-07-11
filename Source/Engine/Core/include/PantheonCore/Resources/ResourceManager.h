#pragma once
#include <filesystem>
#include <string>
#include <unordered_map>

namespace PantheonEngine::Core::Assets
{
    class AssetBundle;
}

namespace PantheonEngine::Core::Resources
{
    class IResource;

    class ResourceManager
    {
        using ResourcePtr = IResource*;
        using ResourceMap = std::unordered_map<std::string, ResourcePtr>;

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
         * \brief Adds all the resources contained in the given asset bundle
         * \param bundle The asset bundle from which resources should be loaded
         */
        void importBundle(const Assets::AssetBundle& bundle);

        /**
         * \brief Tries to load the resource with the given path
         * and stores it in the resource map with the given key.
         * \tparam T The resource's type
         * \param key The resource's key in the resource map
         * \param resourcePath The path of the resource's file
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* load(const std::string& key, const std::string& resourcePath);

        /**
         * \brief Tries to load the resource with the given path
         * and stores it in the resource map with the given key.
         * \tparam T The resource's type
         * \param key The resource's key in the resource map
         * \param data A pointer to the beginning of the resource's memory buffer
         * \param length The length of the resource's memory buffer
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* load(const std::string& key, const void* data, size_t length);

        /**
         * \brief Tries to create an empty resource of the given type
         * and stores it in the resource map with the given key.
         * \param type The resource's type
         * \param key The resource's key
         * \return A pointer to the created resource on success, nullptr otherwise.
         */
        IResource* create(const std::string& type, const std::string& key);

        /**
         * \brief Tries to find the resource with the given key.
         * \tparam T The resource's type
         * \param key The resource's key
         * \return A pointer to the resource on success, nullptr otherwise.
         */
        template <typename T>
        T* get(const std::string& key) const;

        /**
         * \brief Removes the resource with the given key from the manager
         * \param key The resource's key
         */
        void remove(const std::string& key);

        /**
         * \brief Removes all loaded resources from the manager
         */
        void clear();

    private:
        ResourceMap m_resources;
    };
}

#include "PantheonCore/Resources/ResourceManager.inl"
