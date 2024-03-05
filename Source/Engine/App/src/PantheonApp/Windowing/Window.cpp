#include "PantheonApp/Windowing/Window.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>

#include <GLFW/glfw3.h>

using namespace PantheonApp::Input;

namespace PantheonApp::Windowing
{
    Window::Window(Core::IContext& context, const WindowSettings& settings)
        : m_title(settings.m_title),
        m_size({ settings.m_width, settings.m_height }),
        m_minSize({ settings.m_minWidth, settings.m_minHeight }),
        m_maxSize({ settings.m_maxWidth, settings.m_maxHeight }),
        m_context(&context),
        m_refreshRate(GLFW_DONT_CARE),
        m_isFullScreen(false)
    {
        m_handle = nullptr;
        createHandle(settings);
        updateSizeLimits();

        bindCallbacks();
    }

    void Window::makeCurrentContext() const
    {
        m_context->setMainWindow(m_handle);
    }

    std::string Window::getTitle() const
    {
        return m_title;
    }

    void Window::setTitle(const std::string& title)
    {
        glfwSetWindowTitle(static_cast<GLFWwindow*>(m_handle), title.c_str());
        m_title = title;
    }

    Window::PosT Window::getPosition() const
    {
        return m_pos;
    }

    void Window::setPosition(const PosT pos)
    {
        glfwSetWindowPos(static_cast<GLFWwindow*>(m_handle), pos.m_x, pos.m_y);
    }

    Window::DimensionsT Window::getSize() const
    {
        return m_size;
    }

    void Window::setSize(const DimensionsT size)
    {
        glfwSetWindowSize(static_cast<GLFWwindow*>(m_handle), size.m_x, size.m_y);
    }

    Window::DimensionsT Window::getMinSize() const
    {
        return m_minSize;
    }

    void Window::setMinSize(const DimensionsT size)
    {
        m_minSize = size;
        m_maxSize = max(m_maxSize, size);
        updateSizeLimits();
    }

    Window::DimensionsT Window::getMaxSize() const
    {
        DimensionsT maxSize = m_maxSize;

        if (maxSize.m_x == WindowSettings::DONT_CARE)
            maxSize.m_x = INT_MAX;

        if (maxSize.m_y == WindowSettings::DONT_CARE)
            maxSize.m_y = INT_MAX;

        return maxSize;
    }

    void Window::setMaxSize(DimensionsT size)
    {
        if (size.m_x == INT_MAX)
            size.m_x = WindowSettings::DONT_CARE;

        if (size.m_y == INT_MAX)
            size.m_y = WindowSettings::DONT_CARE;

        m_maxSize = size;
        m_minSize = min(m_minSize, size);
        updateSizeLimits();
    }

    Window::CursorPosT Window::getCursorPosition() const
    {
        double mouseX, mouseY;
        glfwGetCursorPos(static_cast<GLFWwindow*>(m_handle), &mouseX, &mouseY);

        return { mouseX, mouseY };
    }

    void Window::setCursorPosition(const CursorPosT cursorPos) const
    {
        glfwSetCursorPos(static_cast<GLFWwindow*>(m_handle), cursorPos.m_x, cursorPos.m_y);
    }

    void Window::showCursor() const
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_NORMAL);
    }

    void Window::hideCursor() const
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_HIDDEN);
    }

    void Window::disableCursor() const
    {
        glfwSetInputMode(static_cast<GLFWwindow*>(m_handle), GLFW_CURSOR, GLFW_CURSOR_DISABLED);
    }

    bool Window::shouldClose() const
    {
        return glfwWindowShouldClose(static_cast<GLFWwindow*>(m_handle));
    }

    void Window::swapBuffers()
    {
        m_context->swapBuffers();
    }

    void Window::setShouldClose(const bool shouldClose) const
    {
        glfwSetWindowShouldClose(static_cast<GLFWwindow*>(m_handle), shouldClose);
    }

    float Window::getAspect() const
    {
        return static_cast<float>(m_size.m_x) / static_cast<float>(m_size.m_y);
    }

    int Window::getRefreshRate() const
    {
        return m_refreshRate;
    }

    void Window::setRefreshRate(const int refreshRate)
    {
        glfwWindowHint(GLFW_REFRESH_RATE, refreshRate);
        m_refreshRate = refreshRate;
    }

    bool Window::isFullScreen() const
    {
        return m_isFullScreen;
    }

    void Window::setFullScreen(const bool shouldEnable)
    {
        m_isFullScreen = shouldEnable;

        glfwSetWindowMonitor
        (
            static_cast<GLFWwindow*>(m_handle),
            shouldEnable ? glfwGetPrimaryMonitor() : nullptr,
            m_pos.m_x, m_pos.m_y,
            m_size.m_x, m_size.m_y,
            m_refreshRate
        );
    }

    void Window::toggleFullScreen()
    {
        setFullScreen(!m_isFullScreen);
    }

    void Window::createHandle(const WindowSettings& settings)
    {
        m_handle = glfwCreateWindow(settings.m_width, settings.m_height, settings.m_title, nullptr, nullptr);

        ASSERT(m_handle != nullptr, "Failed to create GLFW window");

        glfwGetWindowPos(static_cast<GLFWwindow*>(m_handle), &m_pos.m_x, &m_pos.m_y);

        s_windowsMap[m_handle] = this;
    }

    void Window::bindCallbacks() const
    {
        GLFWwindow* glfwWindow = static_cast<GLFWwindow*>(m_handle);

        glfwSetKeyCallback(glfwWindow, onKey);
        glfwSetMouseButtonCallback(glfwWindow, onMouseButton);
        glfwSetCursorPosCallback(glfwWindow, onCursorMove);

        glfwSetWindowPosCallback(glfwWindow, onMove);
        glfwSetWindowSizeCallback(glfwWindow, onResize);
        glfwSetFramebufferSizeCallback(glfwWindow, onFrameBufferResize);
        glfwSetWindowFocusCallback(glfwWindow, onFocus);
        glfwSetWindowIconifyCallback(glfwWindow, onIconify);
        glfwSetWindowCloseCallback(glfwWindow, onClose);
    }

    void Window::updateSizeLimits() const
    {
        glfwSetWindowSizeLimits(static_cast<GLFWwindow*>(m_handle),
            m_minSize.m_x, m_minSize.m_y,
            m_maxSize.m_x, m_maxSize.m_y
        );
    }

    Window* Window::getInstance(void* window)
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
        if (const Window* window = getInstance(glfwWindow))
            window->m_framebufferResizeEvent.invoke(DimensionsT(width, height));
    }

    void Window::onFocus(GLFWwindow* glfwWindow, const int focused)
    {
        if (const Window* window = getInstance(glfwWindow))
            (focused ? window->m_gainFocusEvent : window->m_lostFocusEvent).invoke();
    }

    void Window::onIconify(GLFWwindow* glfwWindow, const int iconified)
    {
        if (const Window* window = getInstance(glfwWindow))
            (iconified ? window->m_minimizeEvent : window->m_maximizeEvent).invoke();
    }

    void Window::onClose(GLFWwindow* glfwWindow)
    {
        if (const Window* window = getInstance(glfwWindow))
            window->m_closeEvent.invoke();
    }
}
