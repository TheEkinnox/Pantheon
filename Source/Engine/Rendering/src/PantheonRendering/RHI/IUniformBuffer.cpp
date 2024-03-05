#include "PantheonRendering/RHI/IUniformBuffer.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullUniformBuffer.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLUniformBuffer.h"

using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    IUniformBuffer::IUniformBuffer(IUniformBuffer&& other) noexcept
        : m_bindIndex(other.m_bindIndex), m_accessMode(other.m_accessMode)
    {
    }

    IUniformBuffer& IUniformBuffer::operator=(IUniformBuffer&& other) noexcept
    {
        if (&other == this)
            return *this;

        m_bindIndex  = other.m_bindIndex;
        m_accessMode = other.m_accessMode;

        other.m_bindIndex = 0;

        return *this;
    }

    void IUniformBuffer::setBindIndex(const uint32_t bindIndex)
    {
        m_bindIndex = bindIndex;
    }

    void IUniformBuffer::bind(const uint32_t index)
    {
        unbind();
        setBindIndex(index);
        bind();
    }

    std::unique_ptr<IUniformBuffer> IUniformBuffer::create(const EAccessMode accessMode, const uint32_t bindIndex)
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullUniformBuffer>(accessMode, bindIndex);
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLUniformBuffer>(accessMode, bindIndex);
        default:
            ASSERT(false, "Failed to create uniform buffer - Unsupported graphics api");
            return {};
        }
    }

    IUniformBuffer::IUniformBuffer(const EAccessMode accessMode, const uint32_t bindIndex)
        : m_bindIndex(bindIndex), m_accessMode(accessMode)
    {
    }
}
