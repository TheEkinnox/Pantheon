#include "PantheonApp/Core/Context.h"

#include <glad/gl.h>
#include <GLFW/glfw3.h>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/ServiceLocator.h"

using namespace PantheonCore::Debug;
using namespace PantheonCore::Utility;
using namespace PantheonApp::Core::Exceptions;

namespace PantheonApp::Core
{
    /**
     * \brief OpenGL debug output callback
     * (in the cpp to avoid an unnecessary include of glad.h in the header)
     * \param source The debug message's source
     * \param type The debug message's type
     * \param id The debug message's id (error/warning code, etc...)
     * \param severity The debug message's severity
     * \param length The debug message's length
     * \param message The debug message
     * \param userParam Addition user parameters
     */
    void GLAPIENTRY glDebugOutput(uint32_t source, uint32_t type, uint32_t id, uint32_t severity, int length, const char* message,
                                  const void* userParam);

    Context::Context(const bool useVsync, const int sampleCount)
        : m_timer(std::make_unique<Timer>()),
        m_useVsync(useVsync)
    {
        // Initialize and configure glfw
        if (glfwInit() == GLFW_FALSE)
            throw ContextInitFailed("Failed to Init GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, PANTHEON_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, PANTHEON_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, sampleCount);

#ifdef __APPLE__
        // required to compile on OS X
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        setVsync(useVsync);

        m_isInitialized = true;

        ServiceLocator::provide<Timer>(*m_timer);
    }

    Context::~Context()
    {
        if (m_isInitialized)
            glfwTerminate();
    }

    void Context::update()
    {
        m_timer->tick();
        glfwPollEvents();
    }

    bool Context::isInitialized() const
    {
        return m_isInitialized;
    }

    bool Context::getVsync() const
    {
        return m_useVsync;
    }

    void Context::setVsync(const bool useVsync)
    {
        glfwSwapInterval(useVsync ? 1 : 0);
        m_useVsync = useVsync;
    }

    void Context::bindDebugCallback()
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
        logger.print("Debug message (%u): %s\n", id, message);

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
            logger.print("Type: Error");
            break;
        case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
            logger.print("Type: Deprecated Behaviour");
            break;
        case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
            logger.print("Type: Undefined Behaviour");
            break;
        case GL_DEBUG_TYPE_PORTABILITY:
            logger.print("Type: Portability");
            break;
        case GL_DEBUG_TYPE_PERFORMANCE:
            logger.print("Type: Performance");
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
}
