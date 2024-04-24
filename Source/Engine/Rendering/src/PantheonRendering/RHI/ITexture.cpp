#include "PantheonRendering/RHI/ITexture.h"

#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/Null/NullTexture.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLTexture.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/FileSystem.h>

#include <rapidjson/istreamwrapper.h>

#define STB_IMAGE_IMPLEMENTATION
#define STBI_ASSERT(x) ASSERT(x)
#include <stb_image.h>

using namespace PantheonCore::Serialization;
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
        m_loadInfo = other.m_loadInfo;

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
        m_loadInfo = other.m_loadInfo;

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

        if (!CHECK(m_data != nullptr, "Unable to load texture from path \"%s\"", fileName.c_str()))
            return false;

        const std::string metaPath = getMetaPath(fileName);

        if (!pathExists(metaPath))
            return true;

        std::ifstream fs(metaPath);

        if (!CHECK(fs.is_open(), "Unable to open texture meta file at path \"%s\"", metaPath.c_str()))
            return false;

        rapidjson::IStreamWrapper isw(fs);

        rapidjson::Document json;
        json.ParseStream(isw);

        if (!CHECK(!json.HasParseError(), "Unable to parse texture meta data from file - Parse error %d", json.GetParseError()))
            return false;

        return m_loadInfo.fromJson(json);
    }

    bool ITexture::save(const std::string& fileName) const
    {
        rapidjson::StringBuffer buffer;
        JsonWriter              writer(buffer);

        if (!m_loadInfo.toJson(writer) || !ASSUME(writer.IsComplete(), "Failed to save texture data - Produced json is incomplete"))
            return false;

        const std::string metaPath = getMetaPath(fileName);
        std::ofstream     fs(metaPath);

        if (!CHECK(fs.is_open(), "Unable to open texture meta file at path \"%s\"", metaPath.c_str()))
            return false;

        fs << std::string_view(buffer.GetString(), buffer.GetLength());

        return CHECK(!fs.bad(), "Failed to write texture meta data to \"%s\"", metaPath.c_str());
    }

    bool ITexture::toBinary(std::vector<char>& output) const
    {
        if (m_data == nullptr)
            return false;

        if (!m_loadInfo.toBinary(output))
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

        if (!CHECK(memCopy(output.data() + startSize, output.size() - startSize, m_data, bufferSize),
                "Unable to serialize texture - Failed copying data to buffer."))
            return false;

        return true;
    }

    size_t ITexture::fromBinary(const char* data, const size_t length)
    {
        if (m_data != nullptr)
        {
            stbi_image_free(m_data);
            m_data = nullptr;
        }

        if (!CHECK(data && length > 0, "Unable to load texture from memory - Empty buffer"))
            return 0;

        size_t offset = m_loadInfo.fromBinary(data, length);

        if (offset == 0)
            return 0;

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
        m_loadInfo.m_wrapModeU = wrapMode;
    }

    void ITexture::setWrapModeV(const ETextureWrapMode wrapMode)
    {
        m_loadInfo.m_wrapModeV = wrapMode;
    }

    void ITexture::setMinFilter(const ETextureFilter filter)
    {
        m_loadInfo.m_minFilter = filter;
    }

    void ITexture::setMagFilter(const ETextureFilter filter)
    {
        m_loadInfo.m_magFilter = filter;
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
        case EPixelDataFormat::NONE:
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
