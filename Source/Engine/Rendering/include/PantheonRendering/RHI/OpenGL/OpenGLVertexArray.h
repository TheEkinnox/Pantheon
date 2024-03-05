#pragma once
#include "PantheonRendering/RHI/IVertexArray.h"

namespace PantheonRendering::RHI
{
    class OpenGLVertexArray final : public IVertexArray
    {
    public:
        explicit OpenGLVertexArray(const IVertexBuffer& vbo, const IIndexBuffer& ebo);

        OpenGLVertexArray(const OpenGLVertexArray&) = delete;
        OpenGLVertexArray(OpenGLVertexArray&& other) noexcept;
        ~OpenGLVertexArray() override;

        OpenGLVertexArray& operator=(const OpenGLVertexArray&) = delete;
        OpenGLVertexArray& operator=(OpenGLVertexArray&& other) noexcept;

        /**
         * \brief Binds the vertex attributes object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the vertex attributes object from the current context
         */
        void unbind() const override;

    private:
        uint32_t m_id = 0;
    };
}
