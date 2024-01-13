#include "PantheonCore/Assets/AssetBundle.h"

#include <fstream>

#include "PantheonCore/Assets/BundleAsset.h"
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/ByteOrder.h"
#include "PantheonCore/Utility/Compression.h"

using namespace PantheonCore::Utility;

namespace PantheonCore::Assets
{
    AssetBundle::AssetBundle()
        : m_compressionMode(ECompressionMode::NONE), m_compressedDataSize(0)
    {
    }

    AssetBundle::AssetBundle(const std::string& path)
    {
        ASSERT(load(path));
    }

    bool AssetBundle::load(const std::string& path)
    {
        // Reset the bundle's data before loading
        *this = AssetBundle();

        if (path.empty())
        {
            DEBUG_LOG_ERROR("Unable to load asset bundle - empty path");
            return false;
        }

        std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);

        if (!ifs.is_open())
        {
            DEBUG_LOG_ERROR("Unable to load asset bundle - couldn't open file");
            return false;
        }

        header_t headerData;
        ifs.read(reinterpret_cast<char*>(&headerData), HEADER_SIZE);
        headerData = fromBigEndian(headerData);

        m_compressionMode = static_cast<ECompressionMode>(readBits(headerData, COMPRESSION_MODE_BITS, 0));
        m_compressedDataSize = readBits(headerData, DATA_SIZE_BITS, COMPRESSION_MODE_BITS);

        DEBUG_LOG("Header: %d | Compression Mode: %d | Compressed Size: %d", headerData, static_cast<int>(m_compressionMode),
            m_compressedDataSize);

        const std::ifstream::off_type offset = static_cast<std::ifstream::off_type>(m_compressedDataSize + sizeof(headerData));

        ifs.seekg(offset, std::ifstream::beg);

        while (!ifs.eof())
        {
            BundleAsset bundleAsset{};

            ifs >> bundleAsset;
            auto& asset = *bundleAsset.getAsset();

            DEBUG_LOG("\nLoaded bundle asset :\n"
                "\t- Offset: %s\n"
                "\t- Compressed Size: %s\n"
                "\t- Uncompressed Size: %s\n"
                "\t- GUID: %s\n\t- Type: %s\n\t- Path: %s",
                sizeToStr(static_cast<double>(bundleAsset.getBlockStart()), true).c_str(),
                sizeToStr(static_cast<double>(bundleAsset.getBlockSize())).c_str(),
                sizeToStr(static_cast<double>(asset.getSize())).c_str(),
                asset.getGuid(), asset.getType(), asset.getPath()
            );

            m_guidMap[asset.getGuid()] = m_assets.size();
            m_pathMap[asset.getPath()] = m_assets.size();
            m_assets.push_back(bundleAsset);
        }

        m_path = path;

        ifs.close();
        DEBUG_LOG("Successfully loaded asset bundle");

        return true;
    }

    bool AssetBundle::save(const char* path, ECompressionMode compressionMode)
    {
        if (path == nullptr)
            path = m_path.c_str();

        m_compressedDataSize = 0;

        std::ofstream ofs(path, std::ifstream::out | std::ifstream::trunc | std::ifstream::binary);

        if (!ofs.is_open())
        {
            DEBUG_LOG_ERROR("Unable to save asset bundle - couldn't open file");
            return false;
        }

        header_t header = 0;

        // reserve the necessary space for the header
        ofs.write(reinterpret_cast<char*>(&header), HEADER_SIZE);

        for (auto& bundleAsset : m_assets)
        {
            bundleAsset.setBlockStart(m_compressedDataSize);

            std::vector<char> assetBuffer, blockBuffer;

            bundleAsset.getAsset()->getData(assetBuffer);

            const block_t dataSize = assetBuffer.size();
            block_t       blockSize = dataSize;

            if (dataSize > 0)
            {
                // compress file
                blockBuffer.resize(blockSize);
                blockSize = compressData(blockBuffer.data(), blockSize, assetBuffer.data(), dataSize, compressionMode);

                // write compressed block
                ofs.write(blockBuffer.data(), static_cast<std::streamsize>(blockSize));
            }

            bundleAsset.setBlockSize(blockSize);
            m_compressedDataSize += blockSize;
        }

        for (auto& bundleAsset : m_assets)
            ofs << bundleAsset;

        ofs << std::flush;

        header = toBigEndian<header_t>(static_cast<uint8_t>(compressionMode) + (m_compressedDataSize << 2));
        ofs.seekp(0, std::ofstream::beg);
        ofs.write(reinterpret_cast<char*>(&header), 8);

        ofs.close();

        m_path = path;
        m_compressionMode = compressionMode;

        return true;
    }

    void AssetBundle::removeAssetAtPath(const std::string& path)
    {
        const auto it = m_pathMap.find(path);
        if (it == m_pathMap.end())
            return;

        m_guidMap.erase(m_assets[it->second].getAsset()->getGuid());
        m_assets.erase(m_assets.begin() + static_cast<ptrdiff_t>(it->second));
        m_pathMap.erase(it);
    }

    void AssetBundle::removeAssetWithGuid(const std::string& guid)
    {
        const auto it = m_guidMap.find(guid);
        if (it == m_guidMap.end())
            return;

        m_pathMap.erase(m_assets[it->second].getAsset()->getPath());
        m_assets.erase(m_assets.begin() + static_cast<ptrdiff_t>(it->second));
        m_guidMap.erase(it);
    }

    std::vector<std::shared_ptr<const Asset>> AssetBundle::getAssets() const
    {
        std::vector<std::shared_ptr<const Asset>> assets;
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

        return m_assets[it->second].getAsset()->getPath();
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

        block_t fileSize = bundleAsset.getAsset()->getSize();

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

        ASSERT(fileSize > 0, "Decompressed size of a non-empty block should be greater than 0");
        fileBuffer.resize(fileSize);

        return fileBuffer;
    }
}
