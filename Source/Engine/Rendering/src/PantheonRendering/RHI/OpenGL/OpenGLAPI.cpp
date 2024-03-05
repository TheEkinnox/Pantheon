#include "PantheonRendering/RHI/OpenGL/OpenGLAPI.h"

#include "PantheonRendering/Enums/EAccessMode.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>

#include <glad/gl.h>

#include <GLFW/glfw3.h>

using namespace PantheonCore::Debug;
using namespace PantheonRendering::Core;
using namespace PantheonRendering::Enums;

namespace PantheonRendering::RHI
{
    void GLAPIENTRY glDebugOutput(const uint32_t source, const uint32_t type, const uint32_t id, const uint32_t severity, const int,
                                  const char*    message, const void*)
    {
        // ignore non-significant error/warning codes
        // NOTE: Here there are the details with a sample output:
        // - #131169 - Framebuffer detailed info: The driver allocated storage for renderbuffer 2. (severity: low)
        // - #131185 - Buffer detailed info: Buffer object 1 (bound to GL_ELEMENT_ARRAY_BUFFER_ARB, usage hint is GL_ENUM_88e4)
        //             will use VIDEO memory as the source for buffer object operations. (severity: low)
        // - #131218 - Program/shader state performance warning: Vertex shader in program 7 is being recompiled based on GL state. (severity: medium)
        // - #131204 - Texture state usage warning: The texture object (0) bound to texture image unit 0 does not have
        //             a defined base level and cannot be used for texture mapping. (severity: low)
        if (id == 131169 || id == 131185 || id == 131218 || id == 131204)
            return;

        Logger& logger = Logger::getInstance();

        logger.print("---------------\n");
        logger.print("Debug message (%u): %s\n", false, id, message);

        switch (source)
        {
        case GL_DEBUG_SOURCE_API:
            logger.print("Source: API");
            break;
        case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
            logger.print("Source: Window System");
            break;
        case GL_DEBUG_SOURCE_SHADER_COMPILER:
            logger.print("Source: Shader Compiler");
            break;
        case GL_DEBUG_SOURCE_THIRD_PARTY:
            logger.print("Source: Third Party");
            break;
        case GL_DEBUG_SOURCE_APPLICATION:
            logger.print("Source: Application");
            break;
        case GL_DEBUG_SOURCE_OTHER:
        default:
            logger.print("Source: Other");
            break;
        }

        logger.print("\n");

        switch (type)
        {
        case GL_DEBUG_TYPE_ERROR:
            logger.print("Type: Error", true);
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            logger.print("Type: Deprecated Behaviour", true);
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            logger.print("Type: Undefined Behaviour", true);
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            logger.print("Type: Portability", true);
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            logger.print("Type: Performance", true);
            break;
        case GL_DEBUG_TYPE_MARKER:
            logger.print("Type: Marker");
            break;
        case GL_DEBUG_TYPE_PUSH_GROUP:
            logger.print("Type: Push Group");
            break;
        case GL_DEBUG_TYPE_POP_GROUP:
            logger.print("Type: Pop Group");
            break;
        case GL_DEBUG_TYPE_OTHER:
        default:
            logger.print("Type: Other");
            break;
        }

        logger.print("\n");

        switch (severity)
        {
        case GL_DEBUG_SEVERITY_HIGH:
            logger.print("Severity: high");
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            logger.print("Severity: medium");
            break;
        case GL_DEBUG_SEVERITY_LOW:
            logger.print("Severity: low");
            break;
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            logger.print("Severity: notification");
            break;
        default:
            logger.print("Severity: unknown");
            break;
        }

        logger.print("\n\n");
    }

    std::string getGLString(const GLenum name)
    {
        const GLubyte* result = glGetString(name);
        return result ? reinterpret_cast<const char*>(result) : std::string();
    }

    GLenum OpenGLAPI::toGLEnum(const EPixelDataFormat dataFormat)
    {
        switch (dataFormat)
        {
        case EPixelDataFormat::STENCIL_INDEX:
            return GL_STENCIL_INDEX;
        case EPixelDataFormat::DEPTH_COMPONENT:
            return GL_DEPTH_COMPONENT;
        case EPixelDataFormat::DEPTH_STENCIL:
            return GL_DEPTH_STENCIL;
        case EPixelDataFormat::RED:
            return GL_RED;
        case EPixelDataFormat::GREEN:
            return GL_GREEN;
        case EPixelDataFormat::BLUE:
            return GL_BLUE;
        case EPixelDataFormat::ALPHA:
            return GL_ALPHA;
        case EPixelDataFormat::RG:
            return GL_RG;
        case EPixelDataFormat::RGB:
            return GL_RGB;
        case EPixelDataFormat::BGR:
            return GL_BGR;
        case EPixelDataFormat::RGBA:
            return GL_RGBA;
        case EPixelDataFormat::BGRA:
            return GL_BGRA;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const EPixelDataType dataType)
    {
        switch (dataType)
        {
        case EPixelDataType::BYTE:
            return GL_BYTE;
        case EPixelDataType::UNSIGNED_BYTE:
            return GL_UNSIGNED_BYTE;
        case EPixelDataType::SHORT:
            return GL_SHORT;
        case EPixelDataType::UNSIGNED_SHORT:
            return GL_UNSIGNED_SHORT;
        case EPixelDataType::INT:
            return GL_INT;
        case EPixelDataType::UNSIGNED_INT:
            return GL_UNSIGNED_INT;
        case EPixelDataType::FLOAT:
            return GL_FLOAT;
        case EPixelDataType::UNSIGNED_BYTE_3_3_2:
            return GL_UNSIGNED_BYTE_3_3_2;
        case EPixelDataType::UNSIGNED_BYTE_2_3_3_REV:
            return GL_UNSIGNED_BYTE_2_3_3_REV;
        case EPixelDataType::UNSIGNED_SHORT_5_6_5:
            return GL_UNSIGNED_SHORT_5_6_5;
        case EPixelDataType::UNSIGNED_SHORT_5_6_5_REV:
            return GL_UNSIGNED_SHORT_5_6_5_REV;
        case EPixelDataType::UNSIGNED_SHORT_4_4_4_4:
            return GL_UNSIGNED_SHORT_4_4_4_4;
        case EPixelDataType::UNSIGNED_SHORT_4_4_4_4_REV:
            return GL_UNSIGNED_SHORT_4_4_4_4_REV;
        case EPixelDataType::UNSIGNED_SHORT_5_5_5_1:
            return GL_UNSIGNED_SHORT_5_5_5_1;
        case EPixelDataType::UNSIGNED_SHORT_1_5_5_5_REV:
            return GL_UNSIGNED_SHORT_1_5_5_5_REV;
        case EPixelDataType::UNSIGNED_INT_8_8_8_8:
            return GL_UNSIGNED_INT_8_8_8_8;
        case EPixelDataType::UNSIGNED_INT_8_8_8_8_REV:
            return GL_UNSIGNED_INT_8_8_8_8_REV;
        case EPixelDataType::UNSIGNED_INT_10_10_10_2:
            return GL_UNSIGNED_INT_10_10_10_2;
        case EPixelDataType::UNSIGNED_INT_2_10_10_10_REV:
            return GL_UNSIGNED_INT_2_10_10_10_REV;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const EPrimitiveType primitiveType)
    {
        switch (primitiveType)
        {
        case EPrimitiveType::POINTS:
            return GL_POINTS;
        case EPrimitiveType::LINES:
            return GL_LINES;
        case EPrimitiveType::LINE_LOOP:
            return GL_LINE_LOOP;
        case EPrimitiveType::LINE_STRIP:
            return GL_LINE_STRIP;
        case EPrimitiveType::TRIANGLES:
            return GL_TRIANGLES;
        case EPrimitiveType::TRIANGLE_STRIP:
            return GL_TRIANGLE_STRIP;
        case EPrimitiveType::TRIANGLE_FAN:
            return GL_TRIANGLE_FAN;
        case EPrimitiveType::LINES_ADJACENCY:
            return GL_LINES_ADJACENCY;
        case EPrimitiveType::LINE_STRIP_ADJACENCY:
            return GL_LINE_STRIP_ADJACENCY;
        case EPrimitiveType::TRIANGLES_ADJACENCY:
            return GL_TRIANGLES_ADJACENCY;
        case EPrimitiveType::TRIANGLE_STRIP_ADJACENCY:
            return GL_TRIANGLE_STRIP_ADJACENCY;
        case EPrimitiveType::PATCHES:
            return GL_PATCHES;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const ERenderingCapability capability)
    {
        switch (capability)
        {
        case ERenderingCapability::BLEND:
            return GL_BLEND;
        case ERenderingCapability::CULL_FACE:
            return GL_CULL_FACE;
        case ERenderingCapability::DEPTH_TEST:
            return GL_DEPTH_TEST;
        case ERenderingCapability::DITHER:
            return GL_DITHER;
        case ERenderingCapability::POLYGON_OFFSET_FILL:
            return GL_POLYGON_OFFSET_FILL;
        case ERenderingCapability::SAMPLE_ALPHA_TO_COVERAGE:
            return GL_SAMPLE_ALPHA_TO_COVERAGE;
        case ERenderingCapability::SAMPLE_COVERAGE:
            return GL_SAMPLE_COVERAGE;
        case ERenderingCapability::SCISSOR_TEST:
            return GL_SCISSOR_TEST;
        case ERenderingCapability::STENCIL_TEST:
            return GL_STENCIL_TEST;
        case ERenderingCapability::MULTISAMPLE:
            return GL_MULTISAMPLE;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const ECompareAlgorithm algorithm)
    {
        switch (algorithm)
        {
        case ECompareAlgorithm::NEVER:
            return GL_NEVER;
        case ECompareAlgorithm::LESS:
            return GL_LESS;
        case ECompareAlgorithm::EQUAL:
            return GL_EQUAL;
        case ECompareAlgorithm::LESS_EQUAL:
            return GL_LEQUAL;
        case ECompareAlgorithm::GREATER:
            return GL_GREATER;
        case ECompareAlgorithm::NOT_EQUAL:
            return GL_NOTEQUAL;
        case ECompareAlgorithm::GREATER_EQUAL:
            return GL_GEQUAL;
        case ECompareAlgorithm::ALWAYS:
            return GL_ALWAYS;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const ECullFace cullFace)
    {
        switch (cullFace)
        {
        case ECullFace::FRONT:
            return GL_FRONT;
        case ECullFace::BACK:
            return GL_BACK;
        case ECullFace::FRONT_AND_BACK:
            return GL_FRONT_AND_BACK;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const EBlendFactor blendFactor)
    {
        switch (blendFactor)
        {
        case EBlendFactor::ZERO:
            return GL_ZERO;
        case EBlendFactor::ONE:
            return GL_ONE;
        case EBlendFactor::SRC_COLOR:
            return GL_SRC_COLOR;
        case EBlendFactor::ONE_MINUS_SRC_COLOR:
            return GL_ONE_MINUS_SRC_COLOR;
        case EBlendFactor::SRC_ALPHA:
            return GL_SRC_ALPHA;
        case EBlendFactor::ONE_MINUS_SRC_ALPHA:
            return GL_ONE_MINUS_SRC_ALPHA;
        case EBlendFactor::DST_ALPHA:
            return GL_DST_ALPHA;
        case EBlendFactor::ONE_MINUS_DST_ALPHA:
            return GL_ONE_MINUS_DST_ALPHA;
        case EBlendFactor::DST_COLOR:
            return GL_DST_COLOR;
        case EBlendFactor::ONE_MINUS_DST_COLOR:
            return GL_ONE_MINUS_DST_COLOR;
        case EBlendFactor::CONSTANT_COLOR:
            return GL_CONSTANT_COLOR;
        case EBlendFactor::ONE_MINUS_CONSTANT_COLOR:
            return GL_ONE_MINUS_CONSTANT_COLOR;
        case EBlendFactor::CONSTANT_ALPHA:
            return GL_CONSTANT_ALPHA;
        case EBlendFactor::ONE_MINUS_CONSTANT_ALPHA:
            return GL_ONE_MINUS_CONSTANT_ALPHA;
        }

        return GL_INVALID_ENUM;
    }

    GLenum OpenGLAPI::toGLEnum(const EAccessMode accessSpecifier)
    {
        switch (accessSpecifier)
        {
        case EAccessMode::STREAM_DRAW:
            return GL_STREAM_DRAW;
        case EAccessMode::STREAM_READ:
            return GL_STREAM_READ;
        case EAccessMode::STREAM_COPY:
            return GL_STREAM_COPY;
        case EAccessMode::DYNAMIC_DRAW:
            return GL_DYNAMIC_DRAW;
        case EAccessMode::DYNAMIC_READ:
            return GL_DYNAMIC_READ;
        case EAccessMode::DYNAMIC_COPY:
            return GL_DYNAMIC_COPY;
        case EAccessMode::STATIC_DRAW:
            return GL_STATIC_DRAW;
        case EAccessMode::STATIC_READ:
            return GL_STATIC_READ;
        case EAccessMode::STATIC_COPY:
            return GL_STATIC_COPY;
        default:
            ASSERT(false, "Invalid access specifier");
            return GL_INVALID_ENUM;
        }
    }

    OpenGLAPI& OpenGLAPI::init(const bool enableDebug)
    {
        const int version = gladLoadGL(glfwGetProcAddress);

        ASSERT(version != 0, "Failed to initialize GLAD");
        DEBUG_LOG("Successfuly initialized OpenGL %d.%d", GLAD_VERSION_MAJOR(version), GLAD_VERSION_MINOR(version));

        if (enableDebug)
        {
            GLint flags = 0;
            glGetIntegerv(GL_CONTEXT_FLAGS, &flags);

            if (flags & GL_CONTEXT_FLAG_DEBUG_BIT)
            {
                glEnable(GL_DEBUG_OUTPUT);
                glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
                glDebugMessageCallback(glDebugOutput, nullptr);
                glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
            }
        }

        return *this;
    }

    OpenGLAPI& OpenGLAPI::clear(const bool clearColor, const bool clearDepth, const bool clearStencil)
    {
        glClear
        (
            (clearColor ? GL_COLOR_BUFFER_BIT : 0) |
            (clearDepth ? GL_DEPTH_BUFFER_BIT : 0) |
            (clearStencil ? GL_STENCIL_BUFFER_BIT : 0)
        );

        return *this;
    }

    OpenGLAPI& OpenGLAPI::readPixels(const PosT&          position, const DimensionsT& size, const EPixelDataFormat dataFormat,
                                     const EPixelDataType dataType, void*              out)
    {
        glReadPixels(position.m_x, position.m_y, size.m_x, size.m_y, toGLEnum(dataFormat), toGLEnum(dataType), out);
        return *this;
    }

    OpenGLAPI& OpenGLAPI::drawElements(const EPrimitiveType primitiveType, const uint32_t indexCount)
    {
        glDrawElements(toGLEnum(primitiveType), static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr);
        return *this;
    }

    OpenGLAPI& OpenGLAPI::drawElementsInstanced(const EPrimitiveType primitiveType, const uint32_t indexCount,
                                                const uint32_t       instances)
    {
        glDrawElementsInstanced(toGLEnum(primitiveType), static_cast<GLsizei>(indexCount), GL_UNSIGNED_INT, nullptr,
            static_cast<GLsizei>(instances));
        return *this;
    }

    OpenGLAPI& OpenGLAPI::drawArrays(const EPrimitiveType primitiveType, const uint32_t vertexCount)
    {
        glDrawArrays(toGLEnum(primitiveType), 0, static_cast<GLsizei>(vertexCount));
        return *this;
    }

    OpenGLAPI& OpenGLAPI::drawArraysInstanced(const EPrimitiveType primitiveType, const uint32_t vertexCount,
                                              const uint32_t       instances)
    {
        glDrawArraysInstanced(toGLEnum(primitiveType), 0, static_cast<GLsizei>(vertexCount), static_cast<GLsizei>(instances));
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setCapability(const ERenderingCapability capability, const bool enable)
    {
        (enable ? glEnable : glDisable)(toGLEnum(capability));
        return *this;
    }

    bool OpenGLAPI::hasCapability(const ERenderingCapability capability)
    {
        return glIsEnabled(toGLEnum(capability));
    }

    OpenGLAPI& OpenGLAPI::setDepthAlgorithm(const ECompareAlgorithm algorithm)
    {
        glDepthFunc(toGLEnum(algorithm));
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setStencilAlgorithm(const ECompareAlgorithm algorithm, const int32_t reference, const uint32_t mask)
    {
        glStencilFunc(toGLEnum(algorithm), reference, mask);
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setClearColor(const Color& color)
    {
        glClearColor(color.m_r, color.m_g, color.m_b, color.m_a);
        return *this;
    }

    Color OpenGLAPI::getClearColor()
    {
        Color clearColor;
        glGetFloatv(GL_COLOR_CLEAR_VALUE, &clearColor.m_r);

        return clearColor;
    }

    OpenGLAPI& OpenGLAPI::setCullFace(const ECullFace cullFace)
    {
        glCullFace(toGLEnum(cullFace));
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setDepthWriting(const bool enable)
    {
        glDepthMask(enable);
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setColorWriting(const bool enableRed, const bool enableGreen, const bool enableBlue,
                                          const bool enableAlpha)
    {
        glColorMask(enableRed, enableGreen, enableBlue, enableAlpha);
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setBlendFunc(const EBlendFactor sourceFactor, const EBlendFactor destinationFactor)
    {
        glBlendFunc(toGLEnum(sourceFactor), toGLEnum(destinationFactor));
        return *this;
    }

    OpenGLAPI& OpenGLAPI::setViewport(const PosT position, const DimensionsT size)
    {
        glViewport(position.m_x, position.m_y, size.m_x, size.m_y);
        return *this;
    }

    EGraphicsAPI OpenGLAPI::getBackend()
    {
        return EGraphicsAPI::OPENGL;
    }

    std::string OpenGLAPI::getVendor()
    {
        return getGLString(GL_VENDOR);
    }

    std::string OpenGLAPI::getHardware()
    {
        return getGLString(GL_RENDERER);
    }

    std::string OpenGLAPI::getVersion()
    {
        return getGLString(GL_VERSION);
    }

    std::string OpenGLAPI::getShadingLanguageVersion()
    {
        return getGLString(GL_SHADING_LANGUAGE_VERSION);
    }
}
