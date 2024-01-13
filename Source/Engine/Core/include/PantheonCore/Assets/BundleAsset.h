#pragma once
#include <cstdint>
#include <ostream>

#include "PantheonCore/Assets/Asset.h"
#include "PantheonCore/utility/macros.h"

namespace PantheonCore::Assets
{
    class BundleAsset
    {
    public:
        static constexpr int BLOCK_SIZE_BITS = 62;
        static constexpr int BLOCK_START_BITS = BLOCK_SIZE_BITS + 1;

        using block_t = SMALLEST_UNSIGNED_TYPE(BLOCK_SIZE_BITS);

        /**
         * \brief Creates a default bundle asset
         */
        BundleAsset();

        /**
         * \brief Creates a bundle asset from the given asset
         * \param asset The asset of which this bundle asset depends
         */
        explicit BundleAsset(const std::shared_ptr<Asset>& asset);

        /**
         * \brief Provides read access to the bundle asset's block start in it's containing bundle
         * \return the start of the asset's block in it's containing bundle
         */
        block_t getBlockStart() const;

        /**
         * \brief Provides write access to the bundle asset's block start in it's containing bundle
         * \param startPos the start of the asset's block in it's containing bundle
         */
        void setBlockStart(block_t startPos);

        /**
         * \brief Provides read access to the bundle asset's block size in it's containing bundle
         * \return the size of the asset's block in it's containing bundle
         */
        block_t getBlockSize() const;

        /**
         * \brief Provides write access to the bundle asset's block size in it's containing bundle
         * \param size the size of the asset's block in it's containing bundle
         */
        void setBlockSize(block_t size);

        /**
         * \brief Provides read access to the asset linked to this bundle asset
         * \return A reference to the asset linked to this bundle asset
         */
        std::shared_ptr<Asset> getAsset();

        /**
         * \brief Provides read access to the asset linked to this bundle asset
         * \return A reference to the asset linked to this bundle asset
         */
        std::shared_ptr<const Asset> getAsset() const;

        /**
         * \brief Reads the bundle asset's data from the given input stream
         * \param is The stream to read the bundle asset data from
         * \param bundleAsset The bundle asset in which the data should be read
         * \return A reference to the input stream
         */
        friend std::istream& operator>>(std::istream& is, BundleAsset& bundleAsset);

        /**
         * \brief Writes the given bundle asset in the given output stream
         * \param os The stream in which the bundle asset should be written
         * \param bundleAsset The bundle asset to write
         * \return A reference to the output stream
         */
        friend std::ostream& operator<<(std::ostream& os, const BundleAsset& bundleAsset);

    private:
        block_t                m_blockStart;
        block_t                m_blockSize;
        std::shared_ptr<Asset> m_asset;
    };
}
