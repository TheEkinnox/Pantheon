#pragma once
#include "PantheonRendering/RHI/IShaderStorageBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLShaderStorageBuffer final : public IShaderStorageBuffer
    {
    public:
        /**
         * \brief Creates a shader storage buffer with the given access mode and bind index
         * \param accessMode The ssbo's access specifier
         * \param bindIndex The ssbo's binding point
         */
        explicit OpenGLShaderStorageBuffer(Enums::EAccessMode accessMode, uint32_t bindIndex);

        /**
         * \brief Disable shader storage buffer copying
         */
        OpenGLShaderStorageBuffer(const OpenGLShaderStorageBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given shader storage buffer
         * \param other The ssbo to move
         */
        OpenGLShaderStorageBuffer(OpenGLShaderStorageBuffer&& other) noexcept;

        /**
         * \brief Destroys the given shader storage buffer
         */
        ~OpenGLShaderStorageBuffer() override;

        /**
         * \brief Disable shader storage buffer copying
         */
        OpenGLShaderStorageBuffer& operator=(const OpenGLShaderStorageBuffer&) = delete;

        /**
         * \brief Moves the given shader storage buffer into this one
         * \param other The ssbo to move
         * \return A reference to the modified ssbo
         */
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
