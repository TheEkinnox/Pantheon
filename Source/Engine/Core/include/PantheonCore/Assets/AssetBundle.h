﻿#pragma once
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
        static constexpr int HEADER_SIZE = (COMPRESSION_MODE_BITS + DATA_SIZE_BITS) / CHAR_BIT;

        using block_t = SMALLEST_UNSIGNED_TYPE(DATA_SIZE_BITS);
        using header_t = SMALLEST_UNSIGNED_TYPE(HEADER_SIZE * CHAR_BIT);

        /**
         * \brief Creates an empty asset bundle
         */
        AssetBundle();

        /**
         * \brief Loads the asset bundle from the given path
         * \param path The path of the asset bundle to load
         */
        void load(const char* path);

        /**
         * \brief Saves the asset bundle at the given path
         * \param path The path at which the asset bundle should be saved
         * \param compressionMode The asset bundle's compression mode
         */
        void save(const char* path, Utility::ECompressionMode compressionMode);

        /**
         * \brief Adds the given asset to the asset bundle
         * \param asset The asset to add to the bundle
         */
        void add(const Asset& asset);

        /**
         * \brief Provides read access to the bundle's assets list
         * \return A vector containing the bundle's assets
         */
        std::vector<Asset> getAssets() const;

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
        std::vector<char> getAssetAtPath(const char* path) const;

        /**
         * \brief Tries to find the asset with the given guid
         * \param guid The guid of the asset to find
         * \return A vector containing the asset's data on success or an empty vector otherwise
         */
        std::vector<char> getAssetWithGuid(const char* guid) const;

        /**
         * \brief Finds the path of the asset with the given guid
         * \param guid The guid of the asset of which path should be found
         * \return The asset's path on success, nullptr otherwise
         */
        const char* getAssetPathFromGuid(const char* guid) const;

    private:
        const char* m_path;
        // 2 bits. Max value is 3 (4 possible options)
        Utility::ECompressionMode m_compressionMode:COMPRESSION_MODE_BITS; // 62 bits. Max value is 524 288 TB (2^62-1 bits)
        block_t m_compressedDataSize:DATA_SIZE_BITS; // 62 bits. Max value is 524 288 TB (2^62-1 bits)
        std::vector<BundleAsset> m_assets;

        std::unordered_map<std::string, size_t> m_guidMap;
        std::unordered_map<std::string, size_t> m_pathMap;
    };
}