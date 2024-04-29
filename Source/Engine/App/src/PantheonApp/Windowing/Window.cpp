#include "PantheonApp/Windowing/Window.h"

#include "PantheonApp/Core/IContext.h"

#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Debug/Logger.h>

#include <climits>

using namespace PantheonApp::Input;

namespace PantheonApp::Windowing
{
    Window::Window(Core::IContext& context, const WindowSettings& settings)
        : m_title(settings.m_title),
        m_minSize({ settings.m_minWidth, settings.m_minHeight }),
        m_maxSize({ settings.m_maxWidth, settings.m_maxHeight }),
        m_context(&context),
        m_isFullScreen(false)
    {
        m_handle = context.createWindowHandle(settings, this);
        updateSizeLimits();
    }

    Window::~Window()
    {
        m_context->destroyWindowHandle(m_handle);
    }

    void Window::makeMain() const
    {
        m_context->setMainWindow(m_handle);
    }

    Core::IContext& Window::getContext() const
    {
        return *m_context;
    }

    std::string Window::getTitle() const
    {
        return m_title;
    }

    void Window::setTitle(const std::string& title)
    {
        m_context->setTitle(m_handle, title.c_str());
        m_title = title;
    }

    Window::PosT Window::getPosition() const
    {
        return m_context->getPosition(m_handle);
    }

    void Window::setPosition(const PosT pos)
    {
        m_context->setPosition(m_handle, pos);
    }

    Window::DimensionsT Window::getSize() const
    {
        return m_context->getSize(m_handle);
    }

    float Window::getAspect() const
    {
        const DimensionsT size = getSize();
        return static_cast<float>(size.m_x) / static_cast<float>(size.m_y);
    }

    void Window::setSize(const DimensionsT size)
    {
        m_context->setSize(m_handle, size);
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
        return m_context->getCursorPosition(m_handle);
    }

    void Window::setCursorPosition(const CursorPosT cursorPos) const
    {
        m_context->setCursorPosition(m_handle, cursorPos);
    }

    void Window::showCursor() const
    {
        m_context->setCursorMode(m_handle, Core::ECursorMode::NORMAL);
    }

    void Window::hideCursor() const
    {
        m_context->setCursorMode(m_handle, Core::ECursorMode::HIDDEN);
    }

    void Window::disableCursor() const
    {
        m_context->setCursorMode(m_handle, Core::ECursorMode::DISABLED);
    }

    bool Window::shouldClose() const
    {
        return m_context->shouldClose(m_handle);
    }

    void Window::setShouldClose(const bool shouldClose) const
    {
        m_context->setShouldClose(m_handle, shouldClose);
    }

    bool Window::isFullScreen() const
    {
        return m_isFullScreen;
    }

    void Window::setFullScreen(const bool shouldEnable)
    {
        m_isFullScreen = shouldEnable;
        m_context->setFullScreen(m_handle, shouldEnable);
    }

    void Window::toggleFullScreen()
    {
        setFullScreen(!m_isFullScreen);
    }

    void Window::swapBuffers()
    {
        m_context->swapBuffers(m_handle);
    }

    void Window::updateSizeLimits() const
    {
        m_context->setSizeLimits(m_handle, m_minSize, m_maxSize);
    }
}
