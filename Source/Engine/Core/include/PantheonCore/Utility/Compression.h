#pragma once

#include <cstdint>

#include "PantheonCore/Utility/ECompressionMode.h"

#ifndef ZSTD_COMPRESSION_LEVEL
#define ZSTD_COMPRESSION_LEVEL 3
#endif

#ifndef BROTLI_COMPRESSION_QUALITY
#define BROTLI_COMPRESSION_QUALITY 5
#endif

#ifndef LZ4_COMPRESSION_LEVEL
#define LZ4_COMPRESSION_LEVEL 6
#endif

namespace PantheonEngine::Core::Utility
{
    /**
     * \brief Compresses the given data block and writes the resulting compressed data
     * in the given, pre-allocated, destination buffer
     * \param dest The destination buffer (must be pre-allocated)
     * \param destSize The length of the destination buffer
     * \param data The memory buffer containing the data to compress
     * \param dataSize The length of the data buffer
     * \param compressionMode The compression algorithm to use
     * \return The resulting compressed data's size on success, 0 otherwise.
     */
    uint64_t compressData(char* dest, uint64_t destSize, const char* data, uint64_t dataSize,
                          ECompressionMode compressionMode);

    /**
     * \brief Decompresses the given data block and writes the resulting decompressed data
     * in the given, pre-allocated, destination buffer
     * \param dest The destination buffer (must be pre-allocated)
     * \param destSize The length of the destination buffer
     * \param data The memory buffer containing the compressed data
     * \param dataSize The length of the data buffer
     * \param compressionMode The compression algorithm used to compress the data
     * \return The number of bytes decompressed into the destination buffer on success, 0 otherwise.
     */
    uint64_t decompressData(char* dest, uint64_t destSize, const char* data, uint64_t dataSize,
                            ECompressionMode compressionMode);
}
