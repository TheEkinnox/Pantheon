#include "glad/gl.h"

#include "PantheonRendering/RHI/OpenGL/OpenGLFrameBuffer.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLTexture.h"

using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    GLenum toGLEnum(const EFrameBufferAttachment attachment)
    {
        if (attachment > EFrameBufferAttachment::COLOR)
        {
            const uint8_t colorIndex = static_cast<uint8_t>(attachment) - static_cast<uint8_t>(EFrameBufferAttachment::COLOR);
            return colorIndex <= GL_COLOR_ATTACHMENT31 - GL_COLOR_ATTACHMENT0 ? GL_COLOR_ATTACHMENT0 + colorIndex : GL_INVALID_ENUM;
        }

        switch (attachment)
        {
        case EFrameBufferAttachment::DEPTH:
            return GL_DEPTH_ATTACHMENT;
        case EFrameBufferAttachment::STENCIL:
            return GL_STENCIL_ATTACHMENT;
        case EFrameBufferAttachment::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL_ATTACHMENT;
        case EFrameBufferAttachment::COLOR:
            return GL_COLOR_ATTACHMENT0;
        }

        return GL_INVALID_ENUM;
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer()
    {
        glGenFramebuffers(1, &m_id);
    }

    OpenGLFrameBuffer::OpenGLFrameBuffer(OpenGLFrameBuffer&& other) noexcept
        : m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLFrameBuffer::~OpenGLFrameBuffer()
    {
        glDeleteFramebuffers(1, &m_id);
    }

    OpenGLFrameBuffer& OpenGLFrameBuffer::operator=(OpenGLFrameBuffer&& other) noexcept
    {
        if (this == &other)
            return *this;

        glDeleteFramebuffers(1, &m_id);

        m_id       = other.m_id;
        other.m_id = 0;

        return *this;
    }

    void OpenGLFrameBuffer::bind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
    }

    void OpenGLFrameBuffer::unbind()
    {
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::attach(const ITexture& texture, const EFrameBufferAttachment attachment)
    {
        const OpenGLTexture& openGLTexture = dynamic_cast<const OpenGLTexture&>(texture);

        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, toGLEnum(attachment), GL_TEXTURE_2D, openGLTexture.getId(), 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }

    void OpenGLFrameBuffer::detach(const EFrameBufferAttachment attachment)
    {
        glBindFramebuffer(GL_FRAMEBUFFER, m_id);
        glFramebufferTexture2D(GL_FRAMEBUFFER, toGLEnum(attachment), GL_TEXTURE_2D, 0, 0);
        glBindFramebuffer(GL_FRAMEBUFFER, 0);
    }
}
