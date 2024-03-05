#include "PantheonRendering/RHI/OpenGL/OpenGLIndexBuffer.h"

#include <glad/gl.h>

namespace PantheonRendering::RHI
{
    OpenGLIndexBuffer::OpenGLIndexBuffer(const uint32_t* indices, const uint32_t count)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * static_cast<GLsizeiptr>(sizeof(uint32_t)), indices, GL_STATIC_DRAW);
    }

    OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept
        : IIndexBuffer(std::forward<IIndexBuffer&&>(other)), m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLIndexBuffer::~OpenGLIndexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(OpenGLIndexBuffer&& other) noexcept
    {
        if (&other == this)
            return *this;

        glDeleteBuffers(1, &m_id);

        m_id       = other.m_id;
        other.m_id = 0;

        IIndexBuffer::operator=(std::move(other));

        return *this;
    }

    void OpenGLIndexBuffer::bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_id);
    }

    void OpenGLIndexBuffer::unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }
}
