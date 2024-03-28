#pragma once
#include "PantheonRendering/Enums/EAccessMode.h"

#include <cstdint>
#include <memory>

namespace PantheonRendering::RHI
{
    class IUniformBuffer
    {
    public:
        /**
         * \brief Disable uniform buffer copying
         */
        IUniformBuffer(const IUniformBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given uniform buffer
         * \param other The ubo to move
         */
        IUniformBuffer(IUniformBuffer&& other) noexcept;

        /**
         * \brief Destroys the uniform buffer
         */
        virtual ~IUniformBuffer() = default;

        /**
         * \brief Disable uniform buffer copying
         */
        IUniformBuffer& operator=(const IUniformBuffer&) = delete;

        /**
         * \brief Moves the given uniform buffer into this one
         * \param other The ubo to copy
         * \return A reference to the modified ubo
         */
        IUniformBuffer& operator=(IUniformBuffer&& other) noexcept;

        /**
         * \brief Binds the ubo to the current binding point
         */
        virtual void bind() = 0;

        /**
         * \brief Unbinds the ubo from the current binding point
         */
        virtual void unbind() = 0;

        /**
         * \brief Sends the data block to the buffer
         * \param data The data block to send
         * \param size The data block's size in bytes
         */
        virtual void setRawData(const void* data, size_t size) = 0;

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The data block to send
         * \param size The data block's size in bytes
         * \param offset The updated data block's offset
         */
        virtual void setRawSubData(const void* data, size_t size, ptrdiff_t offset) = 0;

        /**
         * \brief Sets the ubo's binding point
         */
        void setBindIndex(uint32_t bindIndex);

        /**
         * \brief Binds the ubo at the given binding point
         */
        void bind(uint32_t index);

        /**
         * \brief Sends the given objects array to the buffer
         * \param data The objects array to send
         * \param count The number of elements in the array
         */
        template <typename T>
        void setData(const T* data, size_t count);

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The objects array to send
         * \param count The number of elements in the array
         * \param offset The updated data block's offset
         */
        template <typename T>
        void setSubData(const T* data, size_t count, ptrdiff_t offset);

        /**
         * \brief Creates a uniform buffer for the current render API with the given access specifier and bind index
         * \param accessMode The buffer's access specifier
         * \param bindIndex The buffer's binding point
         * \return The created uniform buffer
         */
        static std::unique_ptr<IUniformBuffer> create(Enums::EAccessMode accessMode, uint32_t bindIndex);

    protected:
        uint32_t           m_bindIndex;
        Enums::EAccessMode m_accessMode;

        /**
         * \brief Creates a uniform buffer with the given access mode and bind index
         * \param accessMode The ubo's access specifier
         * \param bindIndex The ubo's binding point
         */
        IUniformBuffer(Enums::EAccessMode accessMode, uint32_t bindIndex);
    };
}

#include "PantheonRendering/RHI/IUniformBuffer.inl"
