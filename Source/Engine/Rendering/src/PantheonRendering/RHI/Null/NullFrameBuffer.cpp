#include "PantheonRendering/RHI/Null/NullFrameBuffer.h"

namespace PantheonRendering::RHI
{
    void NullFrameBuffer::bind()
    {
    }

    void NullFrameBuffer::unbind()
    {
    }

    void NullFrameBuffer::attach(const ITexture&, Enums::EFrameBufferAttachment)
    {
    }

    void NullFrameBuffer::detach(Enums::EFrameBufferAttachment)
    {
    }

    void NullFrameBuffer::setReadBuffer(Enums::EFrameBufferTarget)
    {
    }

    void NullFrameBuffer::setDrawBuffer(Enums::EFrameBufferTarget)
    {
    }

    void NullFrameBuffer::setDrawBuffers(const Enums::EFrameBufferTarget*, uint8_t)
    {
    }
}
