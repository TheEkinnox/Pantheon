#pragma once
#include "PantheonRendering/Enums/EAccessMode.h"

#include <cstdint>
#include <memory>

namespace PantheonRendering::RHI
{
    class IShaderStorageBuffer
    {
    public:
        /**
         * \brief Disable shader storage buffer copying
         */
        IShaderStorageBuffer(const IShaderStorageBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given shader storage buffer
         * \param other The ssbo to move
         */
        IShaderStorageBuffer(IShaderStorageBuffer&& other) noexcept;

        /**
         * \brief Destroys the given shader storage buffer
         */
        virtual ~IShaderStorageBuffer() = default;

        /**
         * \brief Disable shader storage buffer copying
         */
        IShaderStorageBuffer& operator=(const IShaderStorageBuffer&) = delete;

        /**
         * \brief Moves the given shader storage buffer into this one
         * \param other The ssbo to move
         * \return A reference to the modified ssbo
         */
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

        /**
         * \brief Creates a shader storage buffer with the given access mode and bind index
         * \param accessMode The ssbo's access specifier
         * \param bindIndex The ssbo's binding point
         */
        IShaderStorageBuffer(Enums::EAccessMode accessMode, uint32_t bindIndex);
    };
}

#include "PantheonRendering/RHI/IShaderStorageBuffer.inl"
