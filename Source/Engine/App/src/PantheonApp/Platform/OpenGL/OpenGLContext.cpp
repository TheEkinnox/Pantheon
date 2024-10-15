#include "PantheonApp/Platform/OpenGL/OpenGLContext.h"

#include "PantheonApp/Windowing/Window.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include <GLFW/glfw3.h>

using namespace PantheonApp::Core;
using namespace PantheonApp::Input;
using namespace PantheonApp::Windowing;
using namespace PantheonCore::Debug;
using namespace PantheonCore::Utility;

namespace PantheonApp::Platform
{
    OpenGLContext::OpenGLContext(const bool useVsync, const int sampleCount, const int refreshRate)
        : m_mainWindow(nullptr), m_refreshRate(refreshRate != WindowSettings::DONT_CARE ? refreshRate : GLFW_DONT_CARE),
        m_useVsync(useVsync)
    {
        // Initialize and configure glfw
#ifdef PTH_USE_ASSERTION
        const auto errorCallback = [](const int error, const char* description)
        {
            DEBUG_LOG_ERROR("GLFW Error %d: %s", error, description);
        };

        glfwSetErrorCallback(errorCallback);
#endif

        [[maybe_unused]] const auto result = glfwInit();
        PTH_ASSERT(result == GLFW_TRUE, "Failed to initialize GLFW");

        glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, PANTHEON_OPENGL_VERSION_MAJOR);
        glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, PANTHEON_OPENGL_VERSION_MINOR);
        glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwWindowHint(GLFW_SAMPLES, sampleCount);

#ifdef __APPLE__
        // required to compile on OS X
        glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

        m_isInitialized = true;
    }

    OpenGLContext::~OpenGLContext()
    {
        if (m_isInitialized)
            glfwTerminate();
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

    int OpenGLContext::getRefreshRate() const
    {
        return m_refreshRate;
    }

    void OpenGLContext::setRefreshRate(const int refreshRate)
    {
        glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
        m_refreshRate = refreshRate;
    }

    Window* OpenGLContext::getMainWindow() const
    {
        const auto it = s_windowsMap.find(m_mainWindow);
        return it != s_windowsMap.end() ? it->second : nullptr;
    }

    void OpenGLContext::update()
    {
        IContext::update();
        glfwPollEvents();
    }

    Window* OpenGLContext::getInstance(void* handle)
    {
        PTH_ASSERT(handle, "Attempted to get the window linked to a null handle");

        const auto it = s_windowsMap.find(static_cast<GLFWwindow*>(handle));
        return it != s_windowsMap.end() ? it->second : nullptr;
    }

    void OpenGLContext::bindEvents(GLFWwindow* handle)
    {
        if (!handle)
            return;

        glfwSetKeyCallback(handle,
            [](GLFWwindow* glfwWindow, const int key, const int scanCode, const int action, const int mods)
            {
                if (const Window* window = getInstance(glfwWindow))
                    window->m_keyEvent.invoke(static_cast<EKey>(key), scanCode,
                        static_cast<EKeyState>(action), static_cast<EInputModifier>(mods));
            }
        );

        glfwSetMouseButtonCallback(handle,
            [](GLFWwindow* glfwWindow, const int button, const int action, const int mods)
            {
                if (const Window* window = getInstance(glfwWindow))
                    window->m_mouseButtonEvent.invoke(static_cast<EMouseButton>(button),
                        static_cast<EMouseButtonState>(action),
                        static_cast<EInputModifier>(mods));
            }
        );

        glfwSetCursorPosCallback(handle,
            [](GLFWwindow* glfwWindow, const double x, const double y)
            {
                if (const Window* window = getInstance(glfwWindow))
                    window->m_cursorMoveEvent.invoke({ x, y });
            }
        );

        glfwSetWindowPosCallback(handle,
            [](GLFWwindow* glfwWindow, const int x, const int y)
            {
                if (Window* window = getInstance(glfwWindow))
                    window->m_moveEvent.invoke({ x, y });
            }
        );

        glfwSetWindowSizeCallback(handle,
            [](GLFWwindow* glfwWindow, const int width, const int height)
            {
                if (Window* window = getInstance(glfwWindow))
                    window->m_resizeEvent.invoke({ width, height });
            }
        );

        glfwSetFramebufferSizeCallback(handle,
            [](GLFWwindow* glfwWindow, const int width, const int height)
            {
                if (Window* window = getInstance(glfwWindow))
                    window->m_framebufferResizeEvent.invoke({ width, height });
            }
        );

        glfwSetWindowFocusCallback(handle,
            [](GLFWwindow* glfwWindow, const int focused)
            {
                if (const Window* window = getInstance(glfwWindow))
                    (focused ? window->m_gainFocusEvent : window->m_lostFocusEvent).invoke();
            }
        );

        glfwSetWindowIconifyCallback(handle,
            [](GLFWwindow* glfwWindow, const int iconified)
            {
                if (const Window* window = getInstance(glfwWindow))
                    (iconified ? window->m_minimizeEvent : window->m_maximizeEvent).invoke();
            }
        );

        glfwSetWindowCloseCallback(handle,
            [](GLFWwindow* glfwWindow)
            {
                if (const Window* window = getInstance(glfwWindow))
                    window->m_closeEvent.invoke();
            }
        );
    }

    void* OpenGLContext::createWindowHandle(const WindowSettings& settings, Window* window)
    {
        GLFWwindow* handle = glfwCreateWindow(settings.m_width, settings.m_height, settings.m_title, nullptr, nullptr);

        PTH_ASSERT(handle != nullptr, "Failed to create GLFW window");

        s_windowsMap[handle] = window;
        bindEvents(handle);

        return handle;
    }

    void OpenGLContext::destroyWindowHandle(void*& handle)
    {
        if (!handle)
            return;

        const auto it = s_windowsMap.find(static_cast<GLFWwindow*>(handle));

        PTH_ASSERT(it != s_windowsMap.end(), "Attempted to destroy a window not created by an OpenGL context");

        s_windowsMap.erase(it);
        glfwDestroyWindow(static_cast<GLFWwindow*>(handle));
        handle = nullptr;
    }

    void OpenGLContext::setMainWindow(void* handle)
    {
        m_mainWindow = static_cast<GLFWwindow*>(handle);
        glfwMakeContextCurrent(m_mainWindow);

        if (m_mainWindow)
            setVsync(m_useVsync);
    }

    void OpenGLContext::swapBuffers(void* handle)
    {
        glfwSwapBuffers(static_cast<GLFWwindow*>(handle));
    }

    void OpenGLContext::setTitle(void* handle, const char* title)
    {
        PTH_ASSERT(handle, "Attempted to set title of null window");

        glfwSetWindowTitle(static_cast<GLFWwindow*>(handle), title);
    }

    void OpenGLContext::setFullScreen(void* handle, bool shouldEnable)
    {
        PTH_ASSERT(handle, "Attempted to set fullscreen mode of null window");

        const Window* window = getInstance(handle);

        const Window::PosT        pos  = window->getPosition();
        const Window::DimensionsT size = window->getSize();

        GLFWwindow* glfwHandle = static_cast<GLFWwindow*>(handle);

        glfwSetWindowMonitor
        (
            glfwHandle,
            shouldEnable ? glfwGetPrimaryMonitor() : nullptr,
            pos.m_x, pos.m_y, size.m_x, size.m_y,
            m_refreshRate
        );
    }

    Window::PosT OpenGLContext::getPosition(void* handle) const
    {
        Window::PosT pos;
        glfwGetWindowPos(static_cast<GLFWwindow*>(handle), &pos.m_x, &pos.m_y);
        return pos;
    }

    void OpenGLContext::setPosition(void* handle, const Window::PosT position)
    {
        glfwSetWindowPos(static_cast<GLFWwindow*>(handle), position.m_x, position.m_y);
    }

    Window::DimensionsT OpenGLContext::getSize(void* handle) const
    {
        Window::DimensionsT size;
        glfwGetWindowSize(static_cast<GLFWwindow*>(handle), &size.m_x, &size.m_y);
        return size;
    }

    void OpenGLContext::setSize(void* handle, const Window::DimensionsT size)
    {
        glfwSetWindowSize(static_cast<GLFWwindow*>(handle), size.m_x, size.m_y);
    }

    void OpenGLContext::setSizeLimits(void* handle, const Window::DimensionsT min, const Window::DimensionsT max)
    {
        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(handle),
            min.m_x, min.m_y,
            max.m_x, max.m_y
        );
    }

    Window::CursorPosT OpenGLContext::getCursorPosition(void* handle) const
    {
        Window::CursorPosT pos;
        glfwGetCursorPos(static_cast<GLFWwindow*>(handle), &pos.m_x, &pos.m_y);
        return pos;
    }

    void OpenGLContext::setCursorPosition(void* handle, const Window::CursorPosT pos)
    {
        glfwSetCursorPos(static_cast<GLFWwindow*>(handle), pos.m_x, pos.m_y);
    }

    int toGLFWCursorMode(const ECursorMode mode)
    {
        switch (mode)
        {
        case ECursorMode::NORMAL:
            return GLFW_CURSOR_NORMAL;
        case ECursorMode::HIDDEN:
            return GLFW_CURSOR_HIDDEN;
        case ECursorMode::DISABLED:
            return GLFW_CURSOR_DISABLED;
        default:
            PTH_ASSERT(false, "Invalid cursor mode");
            return GLFW_INVALID_ENUM;
        }
    }

    void OpenGLContext::setCursorMode(void* handle, const ECursorMode mode)
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(handle), GLFW_CURSOR, toGLFWCursorMode(mode));
    }

    bool OpenGLContext::shouldClose(void* handle) const
    {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(handle));
    }

    void OpenGLContext::setShouldClose(void* handle, bool shouldClose)
    {
        glfwSetWindowShouldClose(static_cast<GLFWwindow*>(handle), shouldClose);
    }
}
