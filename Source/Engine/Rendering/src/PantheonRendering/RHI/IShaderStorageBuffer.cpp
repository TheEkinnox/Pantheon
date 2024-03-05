#include "PantheonRendering/RHI/IShaderStorageBuffer.h"

#include "PantheonCore/Utility/ServiceLocator.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullShaderStorageBuffer.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLShaderStorageBuffer.h"

using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    IShaderStorageBuffer::IShaderStorageBuffer(IShaderStorageBuffer&& other) noexcept
        : m_bindIndex(other.m_bindIndex), m_accessMode(other.m_accessMode)
    {
        other.m_bindIndex = 0;
    }

    IShaderStorageBuffer& IShaderStorageBuffer::operator=(IShaderStorageBuffer&& other) noexcept
    {
        if (&other == this)
            return *this;

        m_bindIndex  = other.m_bindIndex;
        m_accessMode = other.m_accessMode;

        other.m_bindIndex = 0;

        return *this;
    }

    void IShaderStorageBuffer::setBindIndex(const uint32_t bindIndex)
    {
        m_bindIndex = bindIndex;
    }

    void IShaderStorageBuffer::bind(const uint32_t index)
    {
        unbind();
        setBindIndex(index);
        bind();
    }

    std::unique_ptr<IShaderStorageBuffer> IShaderStorageBuffer::create(const EAccessMode accessMode, const uint32_t bindIndex)
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullShaderStorageBuffer>(accessMode, bindIndex);
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLShaderStorageBuffer>(accessMode, bindIndex);
        default:
            ASSERT(false, "Failed to create shader storage buffer - Unsupported graphics api");
            return {};
        }
    }

    IShaderStorageBuffer::IShaderStorageBuffer(const EAccessMode accessMode, const uint32_t bindIndex)
        : m_bindIndex(bindIndex), m_accessMode(accessMode)
    {
    }
}
