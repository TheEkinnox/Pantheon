#pragma once
#include "PantheonRendering/RHI/IShaderStorageBuffer.h"

namespace PantheonRendering::RHI
{
    class NullShaderStorageBuffer final : public IShaderStorageBuffer
    {
    public:
        explicit NullShaderStorageBuffer(Enums::EAccessMode accessSpecifier, uint32_t p_bindIndex);
        NullShaderStorageBuffer(const NullShaderStorageBuffer&)           = delete;
        NullShaderStorageBuffer(NullShaderStorageBuffer&& other) noexcept = default;

        ~NullShaderStorageBuffer() override = default;

        NullShaderStorageBuffer& operator=(const NullShaderStorageBuffer&)           = delete;
        NullShaderStorageBuffer& operator=(NullShaderStorageBuffer&& other) noexcept = default;

        /**
         * \brief Binds the ssbo to the current binding point
         */
        void bind() override;

        /**
         * \brief Unbinds the ssbo from the current binding point
         */
        void unbind() override;

        /**
         * \brief Sends the data block to the buffer
         * \param data The data block to send
         * \param blockSize The size of the sent data block
         */
        void setRawData(const void* data, size_t blockSize) override;

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The data block to send
         * \param size The data block's size in bytes
         * \param offset The updated data block's offset
         */
        void setRawSubData(const void* data, size_t size, ptrdiff_t offset) override;
    };
}
