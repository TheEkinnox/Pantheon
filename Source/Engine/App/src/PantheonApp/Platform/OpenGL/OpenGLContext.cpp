#include "PantheonApp/Platform/OpenGL/OpenGLContext.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include <GLFW/glfw3.h>

using namespace PantheonCore::Debug;
using namespace PantheonCore::Utility;

namespace PantheonApp::Platform
{
    OpenGLContext::OpenGLContext(const bool useVsync, const int sampleCount)
        : m_windowHandle(nullptr), m_useVsync(useVsync)
    {
        // Initialize and configure glfw
        [[maybe_unused]] const auto result = glfwInit();
        ASSERT(result == GLFW_TRUE, "Failed to initialize GLFW");

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
    }

    OpenGLContext::~OpenGLContext()
    {
        if (m_isInitialized)
            glfwTerminate();
    }

    void OpenGLContext::update()
    {
        IContext::update();
        glfwPollEvents();
    }

    bool OpenGLContext::isInitialized() const
    {
        return m_isInitialized;
    }

    bool OpenGLContext::getVsync() const
    {
        return m_useVsync;
    }

    void OpenGLContext::setVsync(const bool useVsync)
    {
        glfwSwapInterval(useVsync ? 1 : 0);
        m_useVsync = useVsync;
    }

    void OpenGLContext::setMainWindow(void* handle)
    {
        glfwMakeContextCurrent(static_cast<GLFWwindow*>(handle));
        m_windowHandle = static_cast<GLFWwindow*>(handle);
    }

    void OpenGLContext::swapBuffers()
    {
        glfwSwapBuffers(m_windowHandle);
    }
}
