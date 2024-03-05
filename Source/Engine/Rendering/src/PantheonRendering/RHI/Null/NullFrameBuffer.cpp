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
}
