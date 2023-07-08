#include "PantheonCore/Assets/AssetBundle.h"

#include "PantheonCore/Assets/BundleAsset.h"
#include "PantheonCore/Utility/Compression.h"
#include "PantheonCore/Utility/utility.h"

#include <fstream>
#include <iostream>

using namespace PantheonEngine::Core::Utility;

namespace PantheonEngine::Core::Assets
{
    AssetBundle::AssetBundle() :
        m_path(nullptr), m_compressionMode(ECompressionMode::NONE), m_compressedDataSize(0)
    {
    }

    void AssetBundle::load(const char* path)
    {
        // Reset the bundle's data before loading
        *this = AssetBundle();

        std::cout << "Loading asset bundle at \"" << path << "\"" << std::endl;

        if (path == nullptr)
            throw std::runtime_error("Unable to load asset bundle - null path");

        std::ifstream ifs(path, std::ifstream::in | std::ifstream::binary);

        if (!ifs.is_open())
            throw std::runtime_error("Unable to load asset bundle - couldn't open file");

        header_t headerData;
        ifs.read(reinterpret_cast<char*>(&headerData), HEADER_SIZE);

        m_compressionMode = static_cast<ECompressionMode>(readBits(headerData, COMPRESSION_MODE_BITS, 0));
        m_compressedDataSize = readBits(headerData, DATA_SIZE_BITS, COMPRESSION_MODE_BITS);
        std::cout << "Header: " << headerData << " | Compression Mode: " << static_cast<int>(m_compressionMode) <<
            " | Compressed Size: " << m_compressedDataSize << std::endl;

        const std::ifstream::off_type offset = static_cast<std::ifstream::off_type>(m_compressedDataSize + sizeof
            headerData);

        ifs.seekg(offset, std::ifstream::beg);

        while (!ifs.eof())
        {
            BundleAsset bundleAsset{};

            ifs >> bundleAsset;

            std::cout << "Loaded bundle asset :\n\t- Start: " << bundleAsset.getBlockStart() <<
                "\n\t- Compressed Size: " <<
                bundleAsset.getBlockSize() <<
                "\n\t- Uncompressed Size: " << bundleAsset.getAsset().getSize() << "\n\t- GUID: " << bundleAsset.
                getAsset().
                getGuid() <<
                "\n\t- Type: " << bundleAsset.getAsset().getType() << "\n\t- Path: " << bundleAsset.getAsset().getPath()
                <<
                std::endl;

            m_guidMap[bundleAsset.getAsset().getGuid()] = m_assets.size();
            m_pathMap[bundleAsset.getAsset().getPath()] = m_assets.size();
            m_assets.push_back(bundleAsset);
        }

        m_path = path;

        ifs.close();
        std::cout << "Successfully loaded asset bundle" << std::endl;
    }

    void AssetBundle::save(const char* path, ECompressionMode compressionMode)
    {
        std::cout << "Saving asset bundle at \"" << path << "\"" << std::endl;

        if (path == nullptr)
            path = m_path;

        m_compressedDataSize = 0;

        std::ofstream ofs(path, std::ifstream::out | std::ifstream::trunc | std::ifstream::binary);

        if (!ofs.is_open())
            throw std::runtime_error("Unable to save asset bundle - couldn't open file");

        uint64_t header = 0;

        // reserve the necessary space for the header
        ofs.write(reinterpret_cast<char*>(&header), 8);

        for (auto& bundleAsset : m_assets)
        {
            bundleAsset.setBlockStart(m_compressedDataSize);

            const Asset& asset = bundleAsset.getAsset();
            std::ifstream file(asset.getPath(), std::ifstream::in | std::ifstream::binary);
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

        std::cout << "Successfully saved asset bundle" << std::endl;
    }

    void AssetBundle::add(const Asset& asset)
    {
        m_assets.emplace_back(asset);
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
        return m_path;
    }

    std::vector<char> AssetBundle::getAssetAtPath(const char* path) const
    {
        if (path == nullptr)
            return {};

        if (!m_pathMap.contains(path))
            return {};

        if (m_path == nullptr)
            return {};

        std::ifstream fs(m_path, std::ifstream::in | std::ifstream::binary);

        if (!fs.good())
            return {};

        const BundleAsset bundleAsset = m_assets[m_pathMap.at(path)];

        const std::streamoff blockStart = static_cast<std::streamoff>(HEADER_SIZE + bundleAsset.getBlockStart());
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

    std::vector<char> AssetBundle::getAssetWithGuid(const char* guid) const
    {
        if (!m_guidMap.contains(guid))
            return {};

        return getAssetAtPath(m_assets[m_guidMap.at(guid)].getAsset().getPath());
    }

    const char* AssetBundle::getAssetPathFromGuid(const char* guid) const
    {
        if (guid == nullptr)
            return nullptr;

        if (!m_guidMap.contains(guid))
            return nullptr;

        return m_assets[m_guidMap.at(guid)].getAsset().getPath();
    }
}
