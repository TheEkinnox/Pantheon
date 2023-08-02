#include "PantheonApp/Windowing/Window.h"
#include "PantheonCore/Debug/Logger.h"

#include <glad/glad.h>
#include <GLFW/glfw3.h>


using namespace PantheonEngine::Application::Input;
using namespace PantheonEngine::Application::Windowing::Exceptions;

namespace PantheonEngine::Application::Windowing
{
    Window::Window(const Core::Context& context, const WindowSettings& settings) :
        m_title(settings.m_title),
        m_size({ settings.m_width, settings.m_height }),
        m_minSize({ settings.m_minWidth, settings.m_minHeight }),
        m_maxSize({ settings.m_maxWidth, settings.m_maxHeight }),
        m_context(context),
        m_refreshRate(GLFW_DONT_CARE),
        m_isFullScreen(false)
    {
        m_glfwWindow = nullptr;
        createGlfwWindow(settings);

        bindCallbacks();
    }

    void Window::createGlfwWindow(const WindowSettings& settings)
    {
        m_glfwWindow = glfwCreateWindow(settings.m_width, settings.m_height, settings.m_title,
            nullptr, nullptr);

        if (m_glfwWindow == nullptr)
        {
            DEBUG_LOG("Failed to create GLFW window\n");
            throw WindowCreationFailed("Failed to create GLFW window");
        }

        glfwGetWindowPos(m_glfwWindow, &m_pos.first, &m_pos.second);

        s_windowsMap[m_glfwWindow] = this;
    }

    void Window::bindCallbacks() const
    {
        glfwSetKeyCallback(m_glfwWindow, onKey);
        glfwSetMouseButtonCallback(m_glfwWindow, onMouseButton);
        glfwSetCursorPosCallback(m_glfwWindow, onCursorMove);

        glfwSetWindowPosCallback(m_glfwWindow, onMove);
        glfwSetWindowSizeCallback(m_glfwWindow, onResize);
        glfwSetFramebufferSizeCallback(m_glfwWindow, onFrameBufferResize);
        glfwSetWindowFocusCallback(m_glfwWindow, onFocus);
        glfwSetWindowIconifyCallback(m_glfwWindow, onIconify);
        glfwSetWindowCloseCallback(m_glfwWindow, onClose);
    }

    void Window::makeCurrentContext() const
    {
        glfwMakeContextCurrent(m_glfwWindow);

        // Load GLAD's OpenGL function pointers
        if (!gladLoadGLLoader(reinterpret_cast<GLADloadproc>(glfwGetProcAddress)))
        {
            DEBUG_LOG("Failed to initialize GLAD\n");
            throw std::runtime_error("Failed to initialize GLAD");
        }
    }

    std::string Window::getTitle() const
    {
        return m_title;
    }

    Window::PosT Window::getPosition() const
    {
        return m_pos;
    }

    void Window::setPosition(const PosT pos)
    {
        glfwSetWindowPos(m_glfwWindow, pos.first, pos.second);
    }

    Window::DimensionsT Window::getSize() const
    {
        return m_size;
    }

    void Window::setSize(const DimensionsT size)
    {
        glfwSetWindowSize(m_glfwWindow, size.first, size.second);
    }

    Window::DimensionsT Window::getMinSize() const
    {
        return m_minSize;
    }

    void Window::setMinSize(const DimensionsT size)
    {
        m_minSize = size;
        updateSizeLimits();
    }

    Window::DimensionsT Window::getMaxSize() const
    {
        return m_maxSize;
    }

    void Window::setMaxSize(const DimensionsT size)
    {
        m_maxSize = size;
        updateSizeLimits();
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(m_glfwWindow);
    }

    void Window::swapBuffers() const
    {
        glfwSwapBuffers(m_glfwWindow);
    }

    void Window::setShouldClose(const bool shouldClose) const
    {
        glfwSetWindowShouldClose(m_glfwWindow, shouldClose);
    }

    float Window::getAspect() const
    {
        return static_cast<float>(m_size.first) / static_cast<float>(m_size.second);
    }

    void Window::showCursor() const
    {
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::hideCursor() const
    {
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void Window::disableCursor() const
    {
        glfwSetInputMode(m_glfwWindow, GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    Window::CursorPosT Window::getCursorPosition() const
    {
        double mouseX, mouseY;
        glfwGetCursorPos(m_glfwWindow, &mouseX, &mouseY);

        return { mouseX, mouseY };
    }

    void Window::updateSizeLimits() const
    {
        glfwSetWindowSizeLimits(m_glfwWindow,
            m_minSize.first, m_minSize.second,
            m_maxSize.first, m_maxSize.second
        );
    }

    Window* Window::getInstance(GLFWwindow* window)
    {
        return s_windowsMap.contains(window) ? s_windowsMap[window] : nullptr;
    }

    void Window::onKey(GLFWwindow* glfwWindow, const int key, const int scanCode, const int action, const int mods)
    {
        if (const Window* window = getInstance(glfwWindow))
            window->m_keyEvent.invoke(static_cast<EKey>(key), scanCode,
                static_cast<EKeyState>(action), static_cast<EInputModifier>(mods));
    }

    void Window::onMouseButton(GLFWwindow* glfwWindow, const int button, const int action, const int mods)
    {
        if (const Window* window = getInstance(glfwWindow))
            window->m_mouseButtonEvent.invoke(static_cast<EMouseButton>(button),
                static_cast<EMouseButtonState>(action),
                static_cast<EInputModifier>(mods));
    }

    void Window::onCursorMove(GLFWwindow* glfwWindow, const double x, const double y)
    {
        if (const Window* window = getInstance(glfwWindow))
            window->m_cursorMoveEvent.invoke({ x, y });
    }

    void Window::onMove(GLFWwindow* glfwWindow, const int x, const int y)
    {
        if (Window* window = getInstance(glfwWindow))
        {
            window->m_pos = { x, y };
            window->m_moveEvent.invoke(window->m_pos);
        }
    }

    void Window::onResize(GLFWwindow* glfwWindow, const int width, const int height)
    {
        if (Window* window = getInstance(glfwWindow))
        {
            const DimensionsT size = { width, height };

            window->m_size = size;
            window->m_resizeEvent.invoke(size);
        }
    }

    void Window::onFrameBufferResize(GLFWwindow* glfwWindow, const int width, const int height)
    {
        if (Window* window = getInstance(glfwWindow))
            window->m_framebufferResizeEvent.invoke(DimensionsT(width, height));
    }

    void Window::onFocus(GLFWwindow* glfwWindow, const int focused)
    {
        if (Window* window = getInstance(glfwWindow))
            (focused ? window->m_gainFocusEvent : window->m_lostFocusEvent).invoke();
    }

    void Window::onIconify(GLFWwindow* glfwWindow, const int iconified)
    {
        if (Window* window = getInstance(glfwWindow))
            (iconified ? window->m_minimizeEvent : window->m_maximizeEvent).invoke();
    }

    void Window::onClose(GLFWwindow* glfwWindow)
    {
        if (Window* window = getInstance(glfwWindow))
            window->m_closeEvent.invoke();
    }
}
