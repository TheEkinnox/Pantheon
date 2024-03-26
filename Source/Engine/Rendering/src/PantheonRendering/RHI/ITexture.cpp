#include "PantheonRendering/RHI/ITexture.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullTexture.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLTexture.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) ASSERT(x)
#include <stb_image.h>

using namespace PantheonCore::Utility;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::RHI;

namespace PantheonCore::Resources
{
    template <>
    ITexture* createResource<ITexture>()
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return new NullTexture();
        case EGraphicsAPI::OPENGL:
            return new OpenGLTexture();
        default:
            ASSERT(false, "Failed to create texture - Unsupported graphics api");
            return nullptr;
        }
    }

    template <>
    ITexture* getDefaultResource<ITexture>()
    {
        return &ITexture::getDefault();
    }
}

namespace PantheonRendering::RHI
{
    ITexture::ITexture(const int width, const int height, const EPixelDataFormat format)
        : m_width(width), m_height(height), m_channels(toChannelCount(format))
    {
    }

    ITexture::ITexture(const ITexture& other)
        : IResource(other), m_width(other.m_width), m_height(other.m_height), m_channels(other.m_channels)
    {
        if (other.m_data == nullptr)
        {
            m_data = nullptr;
            return;
        }

        m_data = stbi_load_from_memory(other.m_data, other.m_width * other.m_height * other.m_channels, &m_width, &m_height,
            reinterpret_cast<int*>(&m_channels), 0);
    }

    ITexture::ITexture(ITexture&& other) noexcept
        : IResource(std::forward<IResource&&>(other)), m_data(other.m_data), m_width(other.m_width), m_height(other.m_height),
        m_channels(other.m_channels)
    {
        other.m_data = nullptr;
    }

    ITexture::~ITexture()
    {
        if (m_data != nullptr)
            stbi_image_free(m_data);
    }

    ITexture& ITexture::operator=(const ITexture& other)
    {
        if (&other == this)
            return *this;

        m_width    = other.m_width;
        m_height   = other.m_height;
        m_channels = other.m_channels;

        if (m_data != nullptr)
            stbi_image_free(m_data);

        if (other.m_data == nullptr)
        {
            m_data = nullptr;
            return *this;
        }

        m_data = stbi_load_from_memory(other.m_data, other.m_width * other.m_height * other.m_channels, &m_width, &m_height,
            reinterpret_cast<int*>(&m_channels), 0);

        return *this;
    }

    ITexture& ITexture::operator=(ITexture&& other) noexcept
    {
        if (&other == this)
            return *this;

        if (m_data != nullptr)
            stbi_image_free(m_data);

        m_width    = other.m_width;
        m_height   = other.m_height;
        m_channels = other.m_channels;
        m_data     = other.m_data;

        m_minFilter = other.m_minFilter;
        m_magFilter = other.m_magFilter;
        m_wrapModeU = other.m_wrapModeU;
        m_wrapModeV = other.m_wrapModeV;

        other.m_data = nullptr;

        return *this;
    }

    ITexture& ITexture::getDefault()
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return NullTexture::getDefault();
        case EGraphicsAPI::OPENGL:
            return OpenGLTexture::getDefault();
        }

        ASSERT(false, "Unsupported render api");
        static NullTexture unreachable;
        return unreachable;
    }

    bool ITexture::load(const std::string& fileName)
    {
        if (m_data != nullptr)
            stbi_image_free(m_data);

        stbi_set_flip_vertically_on_load(true);
        m_data = stbi_load(fileName.c_str(), &m_width, &m_height, reinterpret_cast<int*>(&m_channels), 0);

        if (m_data == nullptr)
        {
            DEBUG_LOG_ERROR("Unable to load texture from path \"%s\"", fileName.c_str());
            return false;
        }

        return true;
    }

    bool ITexture::toBinary(std::vector<char>& output) const
    {
        if (m_data == nullptr)
            return false;

        const uint32_t textureInfo = static_cast<uint32_t>(m_minFilter) | (static_cast<uint32_t>(m_magFilter) << 8)
            | (static_cast<uint32_t>(m_wrapModeU) << 16) | (static_cast<uint32_t>(m_wrapModeU) << 24);

        if (!CHECK(writeNumber(textureInfo, output), "Unable to serialize texture - Failed to write load info"))
            return false;

        const ElemSizeT bufferSize = static_cast<ElemSizeT>(m_width) * m_height * m_channels;
        if (bufferSize == 0)
            return false;

        if (!writeNumber(bufferSize, output))
        {
            DEBUG_LOG_ERROR("Unable to serialize texture - Failed to write buffer size");
            return false;
        }

        const size_t startSize = output.size();
        output.resize(startSize + bufferSize);

        if (memcpy_s(output.data() + startSize, output.size() - startSize, m_data, bufferSize) != 0)
        {
            DEBUG_LOG_ERROR("Unable to serialize texture - Failed copying data to buffer.");
            return false;
        }

        return true;
    }

    size_t ITexture::fromBinary(const char* data, const size_t length)
    {
        if (data == nullptr || length == 0)
        {
            DEBUG_LOG_ERROR("Unable to load texture from memory - Empty buffer");
            return 0;
        }

        if (m_data != nullptr)
        {
            stbi_image_free(m_data);
            m_data = nullptr;
        }

        uint32_t textureInfo;
        size_t   offset = readNumber(textureInfo, data, length);

        if (!CHECK(offset != 0, "Unable to load texture from memory - Failed to read load info"))
            return 0;

        m_minFilter = static_cast<ETextureFilter>(readBits(textureInfo, 8, 0));
        m_magFilter = static_cast<ETextureFilter>(readBits(textureInfo, 8, 8));
        m_wrapModeU = static_cast<ETextureWrapMode>(readBits(textureInfo, 8, 16));
        m_wrapModeV = static_cast<ETextureWrapMode>(readBits(textureInfo, 8, 24));

        ElemSizeT    bufferSize;
        const size_t readBytes = readNumber(bufferSize, data + offset, length - offset);

        if (!CHECK(readBytes != 0, "Unable to load texture from memory - Failed to read buffer size"))
            return 0;

        offset += readBytes;

        if (!CHECK(length > offset && length - offset >= bufferSize, "Unable to load texture from memory - Invalid offset"))
            return 0;

        stbi_set_flip_vertically_on_load(true);
        m_data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(data + offset), static_cast<int>(bufferSize),
            &m_width, &m_height, reinterpret_cast<int*>(&m_channels), 0);

        if (!CHECK(m_data == nullptr, "Unable to load texture from memory"))
            return 0;

        return offset + bufferSize;
    }

    LibMath::Vector2I ITexture::getSize() const
    {
        return { m_width, m_height };
    }

    uint8_t ITexture::GetChannelCount() const
    {
        return m_channels;
    }

    void ITexture::setWrapModeU(const ETextureWrapMode wrapMode)
    {
        m_wrapModeU = wrapMode;
    }

    void ITexture::setWrapModeV(const ETextureWrapMode wrapMode)
    {
        m_wrapModeV = wrapMode;
    }

    void ITexture::setMinFilter(const ETextureFilter filter)
    {
        m_minFilter = filter;
    }

    void ITexture::setMagFilter(const ETextureFilter filter)
    {
        m_magFilter = filter;
    }

    uint8_t ITexture::toChannelCount(const EPixelDataFormat format)
    {
        switch (format)
        {
        case EPixelDataFormat::RED:
        case EPixelDataFormat::GREEN:
        case EPixelDataFormat::BLUE:
        case EPixelDataFormat::ALPHA:
        case EPixelDataFormat::STENCIL_INDEX:
        case EPixelDataFormat::DEPTH_COMPONENT:
            return 1;
        case EPixelDataFormat::RG:
        case EPixelDataFormat::DEPTH_STENCIL:
            return 2;
        case EPixelDataFormat::RGB:
        case EPixelDataFormat::BGR:
            return 3;
        case EPixelDataFormat::RGBA:
        case EPixelDataFormat::BGRA:
            return 4;
        default:
            return 0;
        }
    }

    std::shared_ptr<ITexture> ITexture::create(const int width, const int height, const EPixelDataFormat format)
    {
        switch (IRenderAPI::getCurrent().getBackend())
        {
        case EGraphicsAPI::NONE:
            return std::make_shared<NullTexture>(width, height, format);
        case EGraphicsAPI::OPENGL:
            return std::make_shared<OpenGLTexture>(width, height, format);
        default:
            ASSERT(false, "Failed to create texture - Unsupported graphics api");
            return {};
        }
    }
}
