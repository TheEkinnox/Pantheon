#pragma once
#include "PantheonRendering/RHI/IShaderStorageBuffer.h"

namespace PantheonRendering::RHI
{
    class NullShaderStorageBuffer final : public IShaderStorageBuffer
    {
    public:
        /**
         * \brief Creates a shader storage buffer with the given access mode and bind index
         * \param accessMode The ssbo's access specifier
         * \param bindIndex The ssbo's binding point
         */
        explicit NullShaderStorageBuffer(Enums::EAccessMode accessMode, uint32_t bindIndex);

        /**
         * \brief Disable shader storage buffer copying
         */
        NullShaderStorageBuffer(const NullShaderStorageBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given shader storage buffer
         * \param other The ssbo to move
         */
        NullShaderStorageBuffer(NullShaderStorageBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the given shader storage buffer
         */
        ~NullShaderStorageBuffer() override = default;

        /**
         * \brief Disable shader storage buffer copying
         */
        NullShaderStorageBuffer& operator=(const NullShaderStorageBuffer&) = delete;

        /**
         * \brief Moves the given shader storage buffer into this one
         * \param other The ssbo to move
         * \return A reference to the modified ssbo
         */
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
