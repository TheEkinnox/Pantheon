#pragma once
#include "PantheonRendering/Enums/EFrameBufferAttachment.h"
#include "PantheonRendering/Enums/EFrameBufferTarget.h"

#include <memory>

namespace PantheonRendering::RHI
{
    class ITexture;

    class IFrameBuffer
    {
    public:
        /**
         * \brief Disable frame buffer copying
         */
        IFrameBuffer(const IFrameBuffer&) = delete;

        /**
         * \brief Creates a move copy of the given frame buffer
         * \param other The moved frame buffer
         */
        IFrameBuffer(IFrameBuffer&& other) noexcept = default;

        /**
         * \brief Destroys the given frame buffer
         */
        virtual ~IFrameBuffer() = default;

        /**
         * \brief Disable frame buffer copying
         */
        IFrameBuffer& operator=(const IFrameBuffer&) = delete;

        /**
         * \brief Moves the given frame buffer into this one
         * \param other The moved frame buffer
         * \return A reference to the modified frame buffer
         */
        IFrameBuffer& operator=(IFrameBuffer&& other) noexcept = default;

        /**
         * \brief Binds the frame buffer
         */
        virtual void bind() = 0;

        /**
         * \brief Unbinds the frame buffer
         */
        virtual void unbind() = 0;

        /**
         * \brief Attaches the given texture to the frame buffer
         * \param texture The texture to attach
         * \param attachment The target attachment
         */
        virtual void attach(const ITexture& texture, Enums::EFrameBufferAttachment attachment) = 0;

        /**
         * \brief Detaches the given attachment from the frame buffer
         * \param attachment The attachment to detach
         */
        virtual void detach(Enums::EFrameBufferAttachment attachment) = 0;

        /**
         * \brief Sets the frame buffer's read buffer
         * \param target The target read buffer
         */
        virtual void setReadBuffer(Enums::EFrameBufferTarget target) = 0;

        /**
         * \brief Sets the frame buffer's draw buffers
         * \param targets The target draw buffers
         */
        virtual void setDrawBuffer(Enums::EFrameBufferTarget targets) = 0;

        /**
         * \brief Sets the frame buffer's draw buffers
         * \param targets The target draw buffers
         * \param count The number of targets
         */
        virtual void setDrawBuffers(const Enums::EFrameBufferTarget* targets, uint8_t count) = 0;

        /**
         * \brief Creates a frame buffer for the current render api
         * \return The created frame buffer
         */
        static std::unique_ptr<IFrameBuffer> create();

        /**
         * \brief Binds the default frame buffer for the current render api
         */
        static void bindDefault();

    protected:
        IFrameBuffer() = default;
    };
}
