#pragma once
#include "PantheonRendering/RHI/IIndexBuffer.h"

namespace PantheonRendering::RHI
{
    class OpenGLIndexBuffer final : public IIndexBuffer
    {
    public:
        OpenGLIndexBuffer(const uint32_t* indices, uint32_t count);
        OpenGLIndexBuffer(const OpenGLIndexBuffer&) = delete;
        OpenGLIndexBuffer(OpenGLIndexBuffer&& other) noexcept;
        ~OpenGLIndexBuffer() override;

        OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer&) = delete;
        OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& other) noexcept;

        /**
         * \brief Binds the index buffer object to the current context
         */
        void bind() const override;

        /**
         * \brief Unbinds the index buffer from the current context
         */
        void unbind() const override;

    private:
        uint32_t m_id = 0;
    };
}
