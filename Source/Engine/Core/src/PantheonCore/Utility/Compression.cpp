#include "PantheonCore/Utility/Compression.h"

#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Utility/Copy.h"

#include <lz4hc.h>
#include <zstd.h>
#include <brotli/decode.h>
#include <brotli/encode.h>

namespace PantheonCore::Utility
{
    uint64_t compressData(
        char* dest, const uint64_t destSize, const char* data, const uint64_t dataSize, const ECompressionMode compressionMode)
    {
        switch (compressionMode)
        {
        case ECompressionMode::NONE:
            if (destSize < dataSize || !memCopy(dest, destSize, data, dataSize))
                return 0;

            return dataSize;
        case ECompressionMode::ZSTD:
        {
            const uint64_t compressedSize = ZSTD_compress(dest, destSize, data, dataSize,
                ZSTD_COMPRESSION_LEVEL);

            if (ZSTD_isError(compressedSize) || compressedSize == 0 || compressedSize >= dataSize)
                return compressData(dest, destSize, data, dataSize, ECompressionMode::NONE);

            return compressedSize;
        }
        case ECompressionMode::BROTLI:
        {
            uint64_t  compressedSize = destSize;
            const int result         = BrotliEncoderCompress(BROTLI_COMPRESSION_QUALITY, BROTLI_DEFAULT_WINDOW,
                BrotliEncoderMode::BROTLI_MODE_GENERIC, dataSize, reinterpret_cast<const uint8_t*>(data),
                &compressedSize, reinterpret_cast<uint8_t*>(dest));

            if (result != BROTLI_TRUE || compressedSize == 0 || compressedSize >= dataSize)
                return compressData(dest, destSize, data, dataSize, ECompressionMode::NONE);

            return compressedSize;
        }
        case ECompressionMode::LZ4:
        {
            const uint64_t compressedSize = LZ4_compress_HC(data, dest, static_cast<int>(dataSize),
                static_cast<int>(destSize), LZ4_COMPRESSION_LEVEL);

            if (compressedSize == 0 || compressedSize >= dataSize)
                return compressData(dest, destSize, data, dataSize, ECompressionMode::NONE);

            return compressedSize;
        }
        default:
            PTH_ASSERT(false, "Unsupported compression mode");
            return 0;
        }
    }

    uint64_t decompressData(
        char* dest, const uint64_t destSize, const char* data, const uint64_t dataSize, const ECompressionMode compressionMode)
    {
        switch (compressionMode)
        {
        case ECompressionMode::NONE:
            if (destSize < dataSize || !memCopy(dest, destSize, data, dataSize))
                return 0;

            return dataSize;
        case ECompressionMode::ZSTD:
        {
            if (destSize == dataSize)
                return decompressData(dest, destSize, data, dataSize, ECompressionMode::NONE);

            const size_t result = ZSTD_decompress(dest, destSize, data, dataSize);
            return ZSTD_isError(result) ? 0 : result;
        }
        case ECompressionMode::BROTLI:
        {
            if (destSize == dataSize)
                return decompressData(dest, destSize, data, dataSize, ECompressionMode::NONE);

            uint64_t decompressedSize = destSize;

            const BrotliDecoderResult result = BrotliDecoderDecompress(dataSize, reinterpret_cast<const uint8_t*>(data),
                &decompressedSize, reinterpret_cast<uint8_t*>(dest));

            return result == BROTLI_DECODER_RESULT_SUCCESS ? decompressedSize : 0;
        }
        case ECompressionMode::LZ4:
        {
            if (destSize == dataSize)
                return decompressData(dest, destSize, data, dataSize, ECompressionMode::NONE);

            const auto result = LZ4_decompress_safe(data, dest, static_cast<int>(dataSize), static_cast<int>(destSize));

            return result >= 0 ? result : 0;
        }
        default:
            PTH_ASSERT(false, "Unsupported compression mode");
            return 0;
        }
    }
}
