#pragma once
#include <unordered_map>
#include <vector>

#include "PantheonCore/Assets/BundleAsset.h"
#include "PantheonCore/Utility/ECompressionMode.h"
#include "PantheonCore/Utility/macros.h"

namespace PantheonEngine::Core::Assets
{
    class Asset;

    class AssetBundle
    {
    public:
        static constexpr int COMPRESSION_MODE_BITS = 2;
        static constexpr int DATA_SIZE_BITS = 62;
        static constexpr int HEADER_SIZE = ALIGN(COMPRESSION_MODE_BITS + DATA_SIZE_BITS, CHAR_BIT) / CHAR_BIT;

        using block_t = SMALLEST_UNSIGNED_TYPE(DATA_SIZE_BITS);
        using header_t = SMALLEST_UNSIGNED_TYPE(COMPRESSION_MODE_BITS + DATA_SIZE_BITS);

        /**
         * \brief Creates an empty asset bundle
         */
        AssetBundle();

        /**
         * \brief Creates an asset bundle and loads it from the given path
         */
        explicit AssetBundle(const std::string& path);

        /**
         * \brief Loads the asset bundle from the given path
         * \param path The path of the asset bundle to load
         * \return True on success. False otherwise
         */
        bool load(const std::string& path);

        /**
         * \brief Saves the asset bundle at the given path
         * \param path The path at which the asset bundle should be saved
         * \param compressionMode The asset bundle's compression mode
         * \return True on success. False otherwise
         */
        bool save(const char* path, Utility::ECompressionMode compressionMode);

        /**
         * \brief Adds the given asset to the asset bundle
         * \param asset The asset to add to the bundle
         * \return True on success. False otherwise
         */
        template <typename T>
        bool add(const T& asset);

        /**
         * \brief Removes the asset with the given path from the asset bundle
         * \param path The path of the asset to remove from the bundle
         */
        void removeAssetAtPath(const std::string& path);

        /**
         * \brief Removes the asset with the given guid from the asset bundle
         * \param guid The guid of the asset to remove from the bundle
         */
        void removeAssetWithGuid(const std::string& guid);

        /**
         * \brief Provides read access to the bundle's assets list
         * \return A vector containing the bundle's assets
         */
        std::vector<std::shared_ptr<const Asset>> getAssets() const;

        /**
         * \brief Provides read access to the bundle's path
         * \return The asset bundle's save path
         */
        const char* getPath() const;

        /**
         * \brief Tries to find the asset at the given path
         * \param path The path of the asset to find
         * \return A vector containing the asset's data on success or an empty vector otherwise
         */
        std::vector<char> getAssetAtPath(const std::string& path) const;

        /**
         * \brief Tries to find the asset with the given guid
         * \param guid The guid of the asset to find
         * \return A vector containing the asset's data on success or an empty vector otherwise
         */
        std::vector<char> getAssetWithGuid(const std::string& guid) const;

        /**
         * \brief Finds the path of the asset with the given guid
         * \param guid The guid of the asset of which path should be found
         * \return The asset's path on success, nullptr otherwise
         */
        const char* getAssetPathFromGuid(const std::string& guid) const;

    private:
        std::string m_path;

        Utility::ECompressionMode m_compressionMode    : COMPRESSION_MODE_BITS; // 2 bits. Max value is 3 (4 possible options)
        block_t                   m_compressedDataSize : DATA_SIZE_BITS;        // 62 bits. Max value is 524 288 TB (2^62-1 bits)
        std::vector<BundleAsset>  m_assets;

        std::unordered_map<std::string, size_t> m_guidMap;
        std::unordered_map<std::string, size_t> m_pathMap;

        /**
         * \brief Tries to read the given bundle asset's data
         * \param bundleAsset The bundle asset of which data should be read
         * \return A vector containing the asset's data on success or an empty vector otherwise
         */
        std::vector<char> getAssetData(const BundleAsset& bundleAsset) const;
    };
}

#include "AssetBundle.inl"
