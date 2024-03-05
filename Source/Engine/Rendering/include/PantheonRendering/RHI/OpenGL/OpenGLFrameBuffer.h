#pragma once
#include "PantheonRendering/RHI/IFrameBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLFrameBuffer final : public IFrameBuffer
    {
    public:
        /**
         * \brief Creates a default frame buffer
         */
        OpenGLFrameBuffer();

        /**
         * \brief Disable frame buffer copying
         */
        OpenGLFrameBuffer(const OpenGLFrameBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given frame buffer
         * \param other The moved frame buffer
         */
        OpenGLFrameBuffer(OpenGLFrameBuffer&& other) noexcept;

        /**
         * \brief Destroys the given frame buffer
         */
        ~OpenGLFrameBuffer() override;

        /**
         * \brief Disable frame buffer copying
         */
        OpenGLFrameBuffer& operator=(const OpenGLFrameBuffer&) = delete;

        /**
         * \brief Moves the given frame buffer into this one
         * \param other The moved frame buffer
         * \return A reference to the modified frame buffer
         */
        OpenGLFrameBuffer& operator=(OpenGLFrameBuffer&& other) noexcept;

        /**
         * \brief Binds the frame buffer
         */
        void bind() override;

        /**
         * \brief Unbinds the frame buffer
         */
        void unbind() override;

        /**
         * \brief Attaches the given texture to the frame buffer
         * \param texture The texture to attach
         * \param attachment The target attachment
         */
        void attach(const ITexture& texture, Enums::EFrameBufferAttachment attachment) override;

        /**
         * \brief Detaches the given attachment from the frame buffer
         * \param attachment The attachment to detach
         */
        void detach(Enums::EFrameBufferAttachment attachment) override;

    private:
        uint32_t m_id;
    };
}
