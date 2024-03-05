#include "PantheonRendering/RHI/OpenGL/OpenGLVertexArray.h"

#include <glad/gl.h>

using namespace LibMath;
using namespace PantheonRendering::Geometry;

namespace PantheonRendering::RHI
{
    OpenGLVertexArray::OpenGLVertexArray(const IVertexBuffer& vbo, const IIndexBuffer& ebo)
    {
        glGenVertexArrays(1, &m_id);
        glBindVertexArray(m_id);

        vbo.bind();
        ebo.bind();

        constexpr auto stride = sizeof(Vertex);

        // position attribute
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, m_position)));

        // normal attribute
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, m_normal)));

        // texture coordinates attribute
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, m_textureUV)));

        // tangent attribute
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, m_tangent)));

        // bitangent attribute
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, stride, reinterpret_cast<void*>(offsetof(Vertex, m_bitangent)));

        unbind();
    }

    OpenGLVertexArray::OpenGLVertexArray(OpenGLVertexArray&& other) noexcept
        : IVertexArray(std::forward<IVertexArray&&>(other)), m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLVertexArray::~OpenGLVertexArray()
    {
        glDeleteVertexArrays(1, &m_id);
    }

    OpenGLVertexArray& OpenGLVertexArray::operator=(OpenGLVertexArray&& other) noexcept
    {
        if (&other == this)
            return *this;

        glDeleteVertexArrays(1, &m_id);

        m_id       = other.m_id;
        other.m_id = 0;

        IVertexArray::operator=(std::move(other));

        return *this;
    }

    void OpenGLVertexArray::bind() const
    {
        glBindVertexArray(m_id);
    }

    void OpenGLVertexArray::unbind() const
    {
        glBindVertexArray(0);
    }
}
