#include "PantheonRendering/RHI/OpenGL/OpenGLShaderStorageBuffer.h"

#include "PantheonRendering/Enums/EAccessMode.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLAPI.h"

#include <PantheonCore/Debug/Assertion.h>

#include <algorithm>

#include <glad/gl.h>

using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(const EAccessMode accessSpecifier, const uint32_t bindIndex)
        : IShaderStorageBuffer(accessSpecifier, bindIndex)
    {
        glGenBuffers(1, &m_id);
    }

    OpenGLShaderStorageBuffer::OpenGLShaderStorageBuffer(OpenGLShaderStorageBuffer&& other) noexcept
        : IShaderStorageBuffer(std::forward<IShaderStorageBuffer&&>(other)), m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLShaderStorageBuffer::~OpenGLShaderStorageBuffer()
    {
        glDeleteBuffers(1, &m_id);
    }

    OpenGLShaderStorageBuffer& OpenGLShaderStorageBuffer::operator=(OpenGLShaderStorageBuffer&& other) noexcept
    {
        if (&other == this)
            return *this;

        glDeleteBuffers(1, &m_id);

        m_id       = other.m_id;
        other.m_id = 0;

        IShaderStorageBuffer::operator=(std::move(other));

        return *this;
    }

    void OpenGLShaderStorageBuffer::bind()
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindIndex, m_id);
    }

    void OpenGLShaderStorageBuffer::unbind()
    {
        glBindBufferBase(GL_SHADER_STORAGE_BUFFER, m_bindIndex, 0);
    }

    void OpenGLShaderStorageBuffer::setRawData(const void* data, const size_t size)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        glBufferData(GL_SHADER_STORAGE_BUFFER, static_cast<GLsizeiptr>(size), data, OpenGLAPI::toGLEnum(m_accessMode));
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }

    void OpenGLShaderStorageBuffer::setRawSubData(const void* data, const size_t size, const intptr_t offset)
    {
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, m_id);
        glBufferSubData(GL_SHADER_STORAGE_BUFFER, offset, static_cast<GLsizeiptr>(size), data);
        glBindBuffer(GL_SHADER_STORAGE_BUFFER, 0);
    }
}
