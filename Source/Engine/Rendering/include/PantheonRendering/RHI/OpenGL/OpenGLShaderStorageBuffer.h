#pragma once
#include "PantheonRendering/RHI/IShaderStorageBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLShaderStorageBuffer final : public IShaderStorageBuffer
    {
    public:
        explicit OpenGLShaderStorageBuffer(Enums::EAccessMode accessSpecifier, uint32_t bindIndex);
        OpenGLShaderStorageBuffer(const OpenGLShaderStorageBuffer&) = delete;
        OpenGLShaderStorageBuffer(OpenGLShaderStorageBuffer&& other) noexcept;

        ~OpenGLShaderStorageBuffer() override;

        OpenGLShaderStorageBuffer& operator=(const OpenGLShaderStorageBuffer&) = delete;
        OpenGLShaderStorageBuffer& operator=(OpenGLShaderStorageBuffer&& other) noexcept;

        /**
         * \brief Binds the ssbo to the current binding point
         */
        void bind() override;

        /**
         * \brief Unbinds the ssbo from the current binding point
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
