#pragma once
#include "PantheonRendering/RHI/IUniformBuffer.h"

namespace PantheonRendering::RHI
{
    class NullUniformBuffer final : public IUniformBuffer
    {
    public:
        /**
         * \brief Creates a uniform buffer with the given access mode and bind index
         * \param accessMode The ubo's access specifier
         * \param bindIndex The ubo's binding point
         */
        explicit NullUniformBuffer(Enums::EAccessMode accessMode, uint32_t bindIndex);

        /**
         * \brief Disable uniform buffer copying
         */
        NullUniformBuffer(const NullUniformBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given uniform buffer
         * \param other The ubo to move
         */
        NullUniformBuffer(NullUniformBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the uniform buffer
         */
        ~NullUniformBuffer() override = default;

        /**
         * \brief Disable uniform buffer copying
         */
        NullUniformBuffer& operator=(const NullUniformBuffer&) = delete;

        /**
         * \brief Moves the given uniform buffer into this one
         * \param other The ubo to copy
         * \return A reference to the modified ubo
         */
        NullUniformBuffer& operator=(NullUniformBuffer&& other) noexcept = default;

        /**
         * \brief Binds the ubo to the current binding point
         */
        void bind() override;

        /**
         * \brief Unbinds the ubo from the current binding point
         */
        void unbind() override;

        /**
         * \brief Sends the data block to the buffer
         * \param data The data block to send
         * \param size The data block's size in bytes
         */
        void setRawData(const void* data, size_t size) override;

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The data block to send
         * \param size The data block's size in bytes
         * \param offset The updated data block's offset
         */
        void setRawSubData(const void* data, size_t size, ptrdiff_t offset) override;
    };
}
