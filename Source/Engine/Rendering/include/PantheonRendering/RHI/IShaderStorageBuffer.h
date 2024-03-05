#pragma once
#include "PantheonRendering/Enums/EAccessMode.h"

#include <cstdint>
#include <memory>

namespace PantheonRendering::RHI
{
    class IShaderStorageBuffer
    {
    public:
        IShaderStorageBuffer(const IShaderStorageBuffer&) = delete;
        IShaderStorageBuffer(IShaderStorageBuffer&& other) noexcept;

        virtual ~IShaderStorageBuffer() = default;

        IShaderStorageBuffer& operator=(const IShaderStorageBuffer&) = delete;
        IShaderStorageBuffer& operator=(IShaderStorageBuffer&& other) noexcept;

        /**
         * \brief Binds the ssbo to the current binding point
         */
        virtual void bind() = 0;

        /**
         * \brief Unbinds the ssbo from the current binding point
         */
        virtual void unbind() = 0;

        /**
         * \brief Sends the given data block to the buffer
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
         * \brief Sets the ssbo's binding point
         */
        void setBindIndex(uint32_t bindIndex);

        /**
         * \brief Binds the ssbo at the given binding point
         */
        void bind(uint32_t index);

        /**
         * \brief Sends the data block to the buffer
         * \param data The data block to send
         * \param count The number of elements in the block
         */
        template <typename T>
        void setData(T* data, size_t count);

        /**
         * \brief Sets the buffer's data starting at the given offset
         * \param data The objects array to send
         * \param count The number of elements in the array
         * \param offset The updated data block's offset
         */
        template <typename T>
        void setSubData(const T* data, size_t count, ptrdiff_t offset);

        /**
         * \brief Creates a shader storage buffer for the current render API with the given access specifier
         * \param accessMode The buffer's access specifier
         * \param bindIndex The buffer's binding point
         * \return The created shader storage buffer
         */
        static std::unique_ptr<IShaderStorageBuffer> create(Enums::EAccessMode accessMode, uint32_t bindIndex);

    protected:
        uint32_t           m_bindIndex;
        Enums::EAccessMode m_accessMode;

        IShaderStorageBuffer(Enums::EAccessMode accessMode, uint32_t bindIndex);
    };
}

#include "PantheonRendering/RHI/IShaderStorageBuffer.inl"
