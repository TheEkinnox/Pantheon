#pragma once
#include "PantheonRendering/RHI/IIndexBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLIndexBuffer final : public IIndexBuffer
    {
    public:
        /**
         * \brief Creates an index buffer with the given indices
         * \param indices The array of indices to add to the buffer
         * \param count The number of indices in the buffer
         */
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);

        /**
         * \brief Disable index buffer copying
         */
        OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given index buffer
         * \param other The moved index buffer
         */
        OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept;

        /**
         * \brief Destroys the given index buffer
         */
        ~OpenGLIndexBuffer() override;

        /**
         * \brief Disable index buffer copying
         */
        OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;

        /**
         * \brief Moves the given index buffer into this one
         * \param other The moved index buffer
         * \return A reference to the modified index buffer
         */
        OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept;

        /**
         * \brief Binds the index buffer object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the index buffer from the current context
         */
        void unbind() const override;

    private:
        uint32_t m_id = 0;
    };
}
