#pragma once
#include "PantheonRendering/RHI/IFrameBuffer.h"

namespace PantheonRendering::RHI
{
    class NullFrameBuffer final : public IFrameBuffer
    {
    public:
        /**
         * \brief Creates a default frame buffer
         */
        NullFrameBuffer() = default;

        /**
         * \brief Disable frame buffer copying
         */
        NullFrameBuffer(const NullFrameBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given frame buffer
         * \param other The moved frame buffer
         */
        NullFrameBuffer(NullFrameBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the given frame buffer
         */
        ~NullFrameBuffer() override = default;

        /**
         * \brief Disable frame buffer copying
         */
        NullFrameBuffer& operator=(const NullFrameBuffer&) = delete;

        /**
         * \brief Moves the given frame buffer into this one
         * \param other The moved frame buffer
         * \return A reference to the modified frame buffer
         */
        NullFrameBuffer& operator=(NullFrameBuffer&& other) noexcept = default;

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
    };
}
