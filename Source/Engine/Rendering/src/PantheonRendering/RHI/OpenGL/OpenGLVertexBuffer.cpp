#include "PantheonRendering/RHI/OpenGL/OpenGLVertexBuffer.h"

#include <glad/gl.h>

using namespace PantheonRendering::Geometry;

namespace PantheonRendering::RHI
{
    OpenGLVertexBuffer::OpenGLVertexBuffer(const Vertex* vertices, const uint32_t count)
    {
        glGenBuffers(1, &m_id);
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
        glBufferData(GL_ARRAY_BUFFER, count * static_cast<GLsizeiptr>(sizeof(Vertex)), vertices, GL_STATIC_DRAW);
    }

    OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& other) noexcept
        : IVertexBuffer(std::forward<IVertexBuffer&&>(other)), m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLVertexBuffer::~OpenGLVertexBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(OpenGLVertexBuffer&& other) noexcept
    {
        if (&other == this)
            return *this;

        glDeleteBuffers(1, &m_id);

        m_id       = other.m_id;
        other.m_id = 0;

        IVertexBuffer::operator=(std::move(other));

        return *this;
    }

    void OpenGLVertexBuffer::bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_id);
    }

    void OpenGLVertexBuffer::unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}
