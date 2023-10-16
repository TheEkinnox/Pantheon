#include "PantheonCore/Assets/AssetBundle.h"

#include <fstream>

#include "PantheonCore/Assets/BundleAsset.h"
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/Compression.h"
#include "PantheonCore/Utility/utility.h"

using namespace PantheonEngine::Core::Utility;

namespace PantheonEngine::Core::Assets
{
    AssetBundle::AssetBundle()
        : m_compressionMode(ECompressionMode::NONE), m_compressedDataSize(0)
    {
    }

    AssetBundle::AssetBundle(const std::string& path)
    {
        load(path);
    }

    void AssetBundle::load(const std::string& path)
    {
        // Reset the bundle's data before loading
        *this = AssetBundle();

        ASSERT(!path.empty(), "Unable to load asset bundle - empty path");

        DEBUG_LOG("Loading asset bundle from path \"%s\"", path.c_str());

        std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);

        ASSERT(ifs.is_open(), "Unable to load asset bundle - couldn't open file");

        header_t headerData;
        ifs.read(reinterpret_cast<char*>(&headerData), HEADER_SIZE);

        m_compressionMode = static_cast<ECompressionMode>(readBits(headerData, COMPRESSION_MODE_BITS, 0));
        m_compressedDataSize = readBits(headerData, DATA_SIZE_BITS, COMPRESSION_MODE_BITS);

        DEBUG_LOG("Header: %d | Compression Mode: %d | Compressed Size: %d", headerData, static_cast<int>(m_compressionMode),
            m_compressedDataSize);

        const std::ifstream::off_type offset = static_cast<std::ifstream::off_type>(m_compressedDataSize + sizeof
            headerData);

        ifs.seekg(offset, std::ifstream::beg);

        while (!ifs.eof())
        {
            BundleAsset bundleAsset{};

            ifs >> bundleAsset;

            DEBUG_LOG("Loaded bundle asset :\n\t- Start: %d\n\t- Compressed Size: %d\n\t- Uncompressed Size: %d"
                "\n\t- GUID: %s\n\t- Type: %s\n\t- Path: %s", bundleAsset.getBlockStart(), bundleAsset.getBlockSize(),
                bundleAsset.getAsset().getSize(), bundleAsset.getAsset().getGuid(), bundleAsset.getAsset().getType(),
                bundleAsset.getAsset().getPath());

            m_guidMap[bundleAsset.getAsset().getGuid()] = m_assets.size();
            m_pathMap[bundleAsset.getAsset().getPath()] = m_assets.size();
            m_assets.push_back(bundleAsset);
        }

        m_path = path;

        ifs.close();
        DEBUG_LOG("Successfully loaded asset bundle");
    }

    void AssetBundle::save(const char* path, ECompressionMode compressionMode)
    {
        if (path == nullptr)
            path = m_path.c_str();

        DEBUG_LOG("Saving asset bundle at path \"%s\"", path);

        m_compressedDataSize = 0;

        std::ofstream ofs(path, std::ifstream::out | std::ifstream::trunc | std::ifstream::binary);

        ASSERT(ofs.is_open(), "Unable to save asset bundle - couldn't open file");

        uint64_t header = 0;

        // reserve the necessary space for the header
        ofs.write(reinterpret_cast<char*>(&header), 8);

        for (auto& bundleAsset : m_assets)
        {
            bundleAsset.setBlockStart(m_compressedDataSize);

            const Asset&      asset = bundleAsset.getAsset();
            std::ifstream     file(asset.getPath(), std::ifstream::in | std::ifstream::binary);
            std::vector<char> fileBuffer, blockBuffer;

            // get file size
            file.seekg(0, std::ifstream::end);
            uint64_t fileSize = file.tellg(), blockSize = fileSize;
            file.seekg(0, std::ifstream::beg);

            if (fileSize > 0)
            {
                // read file
                fileBuffer.resize(fileSize);
                file.read(fileBuffer.data(), static_cast<std::streamsize>(fileSize));

                // compress file
                blockBuffer.resize(blockSize);
                blockSize = compressData(blockBuffer.data(), blockSize, fileBuffer.data(), fileSize, compressionMode);

                // write compressed block
                ofs.write(blockBuffer.data(), static_cast<std::streamsize>(blockSize));
            }

            file.close();
            bundleAsset.setBlockSize(blockSize);
            m_compressedDataSize += blockSize;
        }

        for (auto& bundleAsset : m_assets)
            ofs << bundleAsset;

        ofs << std::flush;

        header = static_cast<uint8_t>(compressionMode) + (m_compressedDataSize << 2);
        ofs.seekp(0, std::ofstream::beg);
        ofs.write(reinterpret_cast<char*>(&header), 8);

        ofs.close();

        m_path = path;
        m_compressionMode = compressionMode;

        DEBUG_LOG("Successfully saved asset bundle");
    }

    void AssetBundle::add(const Asset& asset)
    {
        m_assets.emplace_back(asset);
    }

    void AssetBundle::removeAssetAtPath(const std::string& path)
    {
        const auto it = m_pathMap.find(path);
        if (it == m_pathMap.end())
            return;

        auto& asset = m_assets[it->second].getAsset();
        m_guidMap.erase(asset.getGuid());
        m_assets.erase(m_assets.begin() + static_cast<ptrdiff_t>(it->second));
        m_pathMap.erase(it);
    }

    void AssetBundle::removeAssetWithGuid(const std::string& guid)
    {
        const auto it = m_guidMap.find(guid);
        if (it == m_guidMap.end())
            return;

        auto& asset = m_assets[it->second].getAsset();
        m_pathMap.erase(asset.getPath());
        m_assets.erase(m_assets.begin() + static_cast<ptrdiff_t>(it->second));
        m_guidMap.erase(it);
    }

    std::vector<Asset> AssetBundle::getAssets() const
    {
        std::vector<Asset> assets;
        assets.reserve(m_assets.size());

        for (const auto& bundleAsset : m_assets)
            assets.push_back(bundleAsset.getAsset());

        return assets;
    }

    const char* AssetBundle::getPath() const
    {
        return m_path.c_str();
    }

    std::vector<char> AssetBundle::getAssetAtPath(const std::string& path) const
    {
        if (path.empty() || m_path.empty())
            return {};

        const auto it = m_pathMap.find(path);
        if (it == m_pathMap.end())
            return {};

        return getAssetData(m_assets[it->second]);
    }

    std::vector<char> AssetBundle::getAssetWithGuid(const std::string& guid) const
    {
        if (guid.empty() || m_path.empty())
            return {};

        const auto it = m_guidMap.find(guid);
        if (it == m_guidMap.end())
            return {};

        return getAssetData(m_assets[it->second]);
    }

    const char* AssetBundle::getAssetPathFromGuid(const std::string& guid) const
    {
        if (guid.empty())
            return nullptr;

        const auto it = m_guidMap.find(guid);
        if (it == m_guidMap.end())
            return nullptr;

        return m_assets[it->second].getAsset().getPath();
    }

    std::vector<char> AssetBundle::getAssetData(const BundleAsset& bundleAsset) const
    {
        std::ifstream fs(m_path, std::ifstream::in | std::ifstream::binary);

        if (!fs.good())
            return {};

        const std::streamoff       blockStart = static_cast<std::streamoff>(HEADER_SIZE + bundleAsset.getBlockStart());
        const BundleAsset::block_t blockSize = bundleAsset.getBlockSize();

        if (blockSize == 0)
            return {};

        uint64_t fileSize = bundleAsset.getAsset().getSize();

        std::vector<char> blockBuffer, fileBuffer;

        fs.seekg(blockStart, std::ifstream::beg);

        if (!fs.good())
            return {};

        // resize buffers
        blockBuffer.resize(blockSize);
        fileBuffer.resize(fileSize);

        // read compressed block
        fs.read(blockBuffer.data(), static_cast<std::streamsize>(blockSize));
        fs.close();

        // decompress block
        fileSize = decompressData(fileBuffer.data(), fileSize, blockBuffer.data(), blockSize, m_compressionMode);
        fileBuffer.resize(fileSize);

        return fileBuffer;
    }
}
