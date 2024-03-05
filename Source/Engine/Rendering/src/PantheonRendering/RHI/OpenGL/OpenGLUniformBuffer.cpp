#include "PantheonRendering/RHI/OpenGL/OpenGLUniformBuffer.h"

#include "PantheonRendering/Enums/EAccessMode.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLAPI.h"

#include <PantheonCore/Debug/Assertion.h>

#include <algorithm>

#include <glad/gl.h>

using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    OpenGLUniformBuffer::OpenGLUniformBuffer(const EAccessMode accessMode, const uint32_t bindIndex)
        : IUniformBuffer(accessMode, bindIndex)
    {
        glGenBuffers(1, &m_id);
    }

    OpenGLUniformBuffer::OpenGLUniformBuffer(OpenGLUniformBuffer&& other) noexcept
        : IUniformBuffer(std::forward<IUniformBuffer&&>(other)), m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLUniformBuffer::~OpenGLUniformBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    OpenGLUniformBuffer& OpenGLUniformBuffer::operator=(OpenGLUniformBuffer&& other) noexcept
    {
        if (&other == this)
            return *this;

        glDeleteBuffers(1, &m_id);

        m_id       = other.m_id;
        other.m_id = 0;

        IUniformBuffer::operator=(std::move(other));

        return *this;
    }

    void OpenGLUniformBuffer::bind()
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_bindIndex, m_id);
    }

    void OpenGLUniformBuffer::unbind()
    {
        glBindBufferBase(GL_UNIFORM_BUFFER, m_bindIndex, 0);
    }

    void OpenGLUniformBuffer::setRawData(const void* data, const size_t size)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferData(GL_UNIFORM_BUFFER, static_cast<GLsizeiptr>(size), data, OpenGLAPI::toGLEnum(m_accessMode));
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }

    void OpenGLUniformBuffer::setRawSubData(const void* data, const size_t size, const intptr_t offset)
    {
        glBindBuffer(GL_UNIFORM_BUFFER, m_id);
        glBufferSubData(GL_UNIFORM_BUFFER, offset, static_cast<GLsizeiptr>(size), data);
        glBindBuffer(GL_UNIFORM_BUFFER, 0);
    }
}
