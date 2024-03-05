#pragma once
#include "PantheonRendering/RHI/IUniformBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLUniformBuffer final : public IUniformBuffer
    {
    public:
        explicit OpenGLUniformBuffer(Enums::EAccessMode accessSpecifier, uint32_t bindIndex);
        OpenGLUniformBuffer(const OpenGLUniformBuffer&) = delete;
        OpenGLUniformBuffer(OpenGLUniformBuffer&& other) noexcept;

        ~OpenGLUniformBuffer() override;

        OpenGLUniformBuffer& operator=(const OpenGLUniformBuffer&) = delete;
        OpenGLUniformBuffer& operator=(OpenGLUniformBuffer&& other) noexcept;

        /**
         * \brief Binds the ubo to the current binding point
         */
        void bind() override;

        /**
         * \brief Unbinds the ubo from the current binding point
         */
        void unbind() override;

        /**
         * \brief Sends the given data block to the buffer
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

    private:
        uint32_t m_id;
    };
}
