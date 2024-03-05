#pragma once
#include "PantheonRendering/RHI/IIndexBuffer.h"

namespace PantheonRendering::RHI
{
    class NullIndexBuffer final : public IIndexBuffer
    {
    public:
        /**
         * \brief Creates an index buffer with the given indices
         * \param indices The array of indices to add to the buffer
         * \param count The number of indices in the buffer
         */
        NullIndexBuffer(const uint32_t* indices, uint32_t count);

        /**
         * \brief Disable index buffer copying
         */
        NullIndexBuffer(const NullIndexBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given index buffer
         * \param other The moved index buffer
         */
        NullIndexBuffer(NullIndexBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the given index buffer
         */
        ~NullIndexBuffer() override = default;

        /**
         * \brief Disable index buffer copying
         */
        NullIndexBuffer& operator=(const NullIndexBuffer&) = delete;

        /**
         * \brief Moves the given index buffer into this one
         * \param other The moved index buffer
         * \return A reference to the modified index buffer
         */
        NullIndexBuffer& operator=(NullIndexBuffer&& other) noexcept = default;

        /**
         * \brief Binds the index buffer object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the index buffer from the current context
         */
        void unbind() const override;
    };
}
