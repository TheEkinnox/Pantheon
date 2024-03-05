#include "PantheonRendering/RHI/OpenGL/OpenGLTexture.h"

#include "PantheonRendering/RHI/OpenGL/OpenGLAPI.h"

#include <glad/gl.h>

using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    GLint toGLInt(const ETextureFilter filter)
    {
        switch (filter)
        {
        case ETextureFilter::NEAREST:
            return GL_NEAREST;
        case ETextureFilter::LINEAR:
            return GL_LINEAR;
        case ETextureFilter::NEAREST_MIPMAP_NEAREST:
            return GL_NEAREST_MIPMAP_NEAREST;
        case ETextureFilter::LINEAR_MIPMAP_NEAREST:
            return GL_LINEAR_MIPMAP_NEAREST;
        case ETextureFilter::NEAREST_MIPMAP_LINEAR:
            return GL_NEAREST_MIPMAP_LINEAR;
        case ETextureFilter::LINEAR_MIPMAP_LINEAR:
            return GL_LINEAR_MIPMAP_LINEAR;
        default:
            ASSERT(false, "Invalid texture filter");
            return GL_INVALID_ENUM;
        }
    }

    GLint toGLInt(const ETextureWrapMode wrapMode)
    {
        switch (wrapMode)
        {
        case ETextureWrapMode::REPEAT:
            return GL_REPEAT;
        case ETextureWrapMode::MIRRORED_REPEAT:
            return GL_MIRRORED_REPEAT;
        case ETextureWrapMode::CLAMP_TO_EDGE:
            return GL_CLAMP_TO_EDGE;
        case ETextureWrapMode::CLAMP_TO_BORDER:
            return GL_CLAMP_TO_BORDER;
        default:
            ASSERT(false, "Invalid wrap mode");
            return GL_INVALID_ENUM;
        }
    }

    GLenum getGLFormat(const uint8_t channels)
    {
        switch (channels)
        {
        case 1:
            return GL_RED;
        case 2:
            return GL_RG;
        case 3:
            return GL_RGB;
        case 4:
            return GL_RGBA;
        default:
            ASSERT(false, "Invalid channels count. Expected 1-4 but received \"%d\".", channels);
            return GL_INVALID_ENUM;
        }
    }

    OpenGLTexture::OpenGLTexture(const int width, const int height, const EPixelDataFormat format)
        : ITexture(width, height, format)
    {
        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        const GLenum texFormat = OpenGLAPI::toGLEnum(format);
        glTexImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(texFormat), m_width, m_height, 0, texFormat, GL_FLOAT, nullptr);
    }

    OpenGLTexture::OpenGLTexture(const OpenGLTexture& other)
        : ITexture(other)
    {
        if (other.m_id == 0)
        {
            m_id = 0;
            return;
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(m_channels), GL_FLOAT, nullptr);

        glCopyImageSubData(other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_width, m_height, 1
        );
    }

    OpenGLTexture::OpenGLTexture(OpenGLTexture&& other) noexcept
        : ITexture(std::forward<ITexture&&>(other)), m_id(other.m_id)
    {
        other.m_id = 0;
    }

    OpenGLTexture::~OpenGLTexture()
    {
        if (m_id != 0)
            glDeleteTextures(1, &m_id);
    }

    OpenGLTexture& OpenGLTexture::operator=(const OpenGLTexture& other)
    {
        if (this == &other)
            return *this;

        ITexture::operator=(other);

        if (m_id != 0)
            glDeleteTextures(1, &m_id);

        if (other.m_id == 0)
        {
            m_id = 0;
            return *this;
        }

        glGenTextures(1, &m_id);
        glBindTexture(GL_TEXTURE_2D, m_id);

        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(m_channels), GL_FLOAT, nullptr);

        glCopyImageSubData(other.m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_id, GL_TEXTURE_2D, 0, 0, 0, 0,
            m_width, m_height, 1
        );

        return *this;
    }

    OpenGLTexture& OpenGLTexture::operator=(OpenGLTexture&& other) noexcept
    {
        if (m_id != 0)
            glDeleteTextures(1, &m_id);

        m_id = other.m_id;

        other.m_id = 0;

        ITexture::operator=(std::move(other));

        return *this;
    }

    OpenGLTexture& OpenGLTexture::getDefault()
    {
        static OpenGLTexture texture;

        if (texture.m_id == 0)
        {
            texture.m_width    = 1;
            texture.m_height   = 1;
            texture.m_channels = 3;

            glGenTextures(1, &texture.m_id);
            glBindTexture(GL_TEXTURE_2D, texture.m_id);

            static constexpr GLubyte white[3] = { 255, 255, 255 };
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, texture.m_width, texture.m_height, 0, GL_RGB, GL_UNSIGNED_BYTE, white);

            texture.setWrapModeU(ETextureWrapMode::REPEAT);
            texture.setWrapModeV(ETextureWrapMode::REPEAT);

            texture.setMinFilter(ETextureFilter::NEAREST);
            texture.setMagFilter(ETextureFilter::NEAREST);
        }

        return texture;
    }

    bool OpenGLTexture::init()
    {
        if (!CHECK(m_data != nullptr, "Unable to initialize opengl texture - no pixels"))
            return false;

        if (m_id == 0)
        {
            glGenTextures(1, &m_id);

            if (!CHECK(m_id != 0, "Unable to generate opengl texture id."))
                return false;
        }

        glBindTexture(GL_TEXTURE_2D, m_id);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_width, m_height, 0, getGLFormat(m_channels), GL_UNSIGNED_BYTE, m_data);

        glBindTexture(GL_TEXTURE_2D, 0);
        return true;
    }

    void OpenGLTexture::bind(const uint8_t slot)
    {
        glBindTextureUnit(slot, m_id);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, toGLInt(m_wrapModeU));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, toGLInt(m_wrapModeV));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, toGLInt(m_minFilter));
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, toGLInt(m_magFilter));
    }

    void OpenGLTexture::unbind(const uint8_t slot)
    {
        glBindTextureUnit(slot, 0);
    }

    void OpenGLTexture::generateMipmap()
    {
        glGenerateMipmap(GL_TEXTURE_2D);
    }

    uint32_t OpenGLTexture::getId() const
    {
        return m_id;
    }
}
