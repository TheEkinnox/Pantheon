#include "PantheonCore/Assets/BundleAsset.h"

#include <istream>

#include "PantheonCore/Resources/ResourceAsset.h"
#include "PantheonCore/Utility/utility.h"

namespace PantheonCore::Assets
{
    BundleAsset::BundleAsset() : m_blockStart(0), m_blockSize(0)
    {
    }

    BundleAsset::BundleAsset(const std::shared_ptr<Asset>& asset) : m_blockStart(0), m_blockSize(0), m_asset(asset)
    {
    }

    BundleAsset::block_t BundleAsset::getBlockStart() const
    {
        return m_blockStart;
    }

    void BundleAsset::setBlockStart(const block_t startPos)
    {
        m_blockStart = startPos;
    }

    BundleAsset::block_t BundleAsset::getBlockSize() const
    {
        return m_blockSize;
    }

    void BundleAsset::setBlockSize(const block_t size)
    {
        m_blockSize = size;
    }

    std::shared_ptr<Asset> BundleAsset::getAsset()
    {
        return m_asset;
    }

    std::shared_ptr<const Asset> BundleAsset::getAsset() const
    {
        return std::dynamic_pointer_cast<const Asset>(m_asset);
    }

    std::istream& operator>>(std::istream& is, BundleAsset& bundleAsset)
    {
        is.read(reinterpret_cast<char*>(&bundleAsset.m_blockStart), ALIGN(BundleAsset::BLOCK_START_BITS, CHAR_BIT) / CHAR_BIT);
        is.read(reinterpret_cast<char*>(&bundleAsset.m_blockSize), ALIGN(BundleAsset::BLOCK_SIZE_BITS, CHAR_BIT) / CHAR_BIT);

        bundleAsset.m_blockStart = Utility::fromBigEndian(bundleAsset.m_blockStart);
        bundleAsset.m_blockSize = Utility::fromBigEndian(bundleAsset.m_blockSize);

        const bool isResource = Utility::readBits(bundleAsset.m_blockStart, 1, 0) == 1;
        bundleAsset.m_blockStart = Utility::readBits(bundleAsset.m_blockStart, BundleAsset::BLOCK_START_BITS - 1, 1);
        bundleAsset.m_asset = isResource ? std::make_shared<Resources::ResourceAsset>() : std::make_shared<Asset>();
        is >> *bundleAsset.m_asset;

        return is;
    }

    std::ostream& operator<<(std::ostream& os, const BundleAsset& bundleAsset)
    {
        const bool isResource = dynamic_cast<Resources::ResourceAsset*>(bundleAsset.m_asset.get()) != nullptr;

        const BundleAsset::block_t tmp = isResource + (bundleAsset.m_blockStart << 1);

        const auto leStart = Utility::toBigEndian(tmp);
        const auto leSize = Utility::toBigEndian(bundleAsset.m_blockSize);

        os.write(reinterpret_cast<const char*>(&leStart), ALIGN(BundleAsset::BLOCK_START_BITS, CHAR_BIT) / CHAR_BIT);
        os.write(reinterpret_cast<const char*>(&leSize), ALIGN(BundleAsset::BLOCK_SIZE_BITS, CHAR_BIT) / CHAR_BIT);
        os << *bundleAsset.m_asset;

        return os;
    }
}
