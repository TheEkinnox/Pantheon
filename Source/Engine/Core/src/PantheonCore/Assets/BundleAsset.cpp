#include "PantheonCore/Assets/BundleAsset.h"

#include <istream>

namespace PantheonEngine::Core::Assets
{
    BundleAsset::BundleAsset() : m_blockStart(0), m_blockSize(0)
    {
    }

    BundleAsset::BundleAsset(Asset asset) : m_blockStart(0), m_blockSize(0), m_asset(std::move(asset))
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

    const Asset& BundleAsset::getAsset() const
    {
        return m_asset;
    }

    std::istream& operator>>(std::istream& is, BundleAsset& bundleAsset)
    {
        is.read(reinterpret_cast<char*>(&bundleAsset.m_blockStart), BundleAsset::BLOCK_START_BITS / CHAR_BIT);
        is.read(reinterpret_cast<char*>(&bundleAsset.m_blockSize), BundleAsset::BLOCK_SIZE_BITS / CHAR_BIT);
        is >> bundleAsset.m_asset;

        return is;
    }

    std::ostream& operator<<(std::ostream& os, const BundleAsset& bundleAsset)
    {
        os.write(reinterpret_cast<const char*>(&bundleAsset.m_blockStart), BundleAsset::BLOCK_START_BITS / CHAR_BIT);
        os.write(reinterpret_cast<const char*>(&bundleAsset.m_blockSize), BundleAsset::BLOCK_SIZE_BITS / CHAR_BIT);
        os << bundleAsset.m_asset;

        return os;
    }
}
