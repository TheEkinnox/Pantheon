#pragma once
#include "PantheonRendering/RHI/IUniformBuffer.h"

namespace PantheonRendering::RHI
{
    class NullUniformBuffer final : public IUniformBuffer
    {
    public:
        explicit NullUniformBuffer(Enums::EAccessMode accessSpecifier, uint32_t p_bindIndex);
        NullUniformBuffer(const NullUniformBuffer&)     = delete;
        NullUniformBuffer(NullUniformBuffer&& other) noexcept = default;

        ~NullUniformBuffer() override = default;

        NullUniformBuffer& operator=(const NullUniformBuffer&)     = delete;
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
