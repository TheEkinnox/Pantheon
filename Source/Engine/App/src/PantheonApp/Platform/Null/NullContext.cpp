#include "PantheonApp/Platform/Null/NullContext.h"

#include "PantheonCore/Debug/Assertion.h"

namespace PantheonApp::Platform
{
    NullContext::NullContext(bool, int, int)
    {
        m_isInitialized = true;
        m_mainWindow    = 0;
    }

    bool NullContext::isInitialized() const
    {
        return m_isInitialized;
    }

    bool NullContext::getVsync() const
    {
        return false;
    }

    void NullContext::setVsync(bool)
    {
    }

    int NullContext::getRefreshRate() const
    {
        return 0;
    }

    void NullContext::setRefreshRate(int)
    {
    }

    Windowing::Window* NullContext::getMainWindow() const
    {
        const auto it = s_windowsMap.find(m_mainWindow);
        return it != s_windowsMap.end() ? it->second : nullptr;
    }

    void NullContext::update()
    {
        IContext::update();
    }

    void* NullContext::createWindowHandle(const Windowing::WindowSettings&, Windowing::Window* window)
    {
        const size_t index  = s_currentIndex++;
        s_windowsMap[index] = window;
        return (void*)index;
    }

    void NullContext::destroyWindowHandle(void*& handle)
    {
        const size_t index = reinterpret_cast<size_t>(handle);

        if (index == 0)
            return;

        const auto it = s_windowsMap.find(index);

        ASSERT(it != s_windowsMap.end(), "Attempted to destroy a window not created by a null context");

        s_windowsMap.erase(it);
        handle = nullptr;
    }

    void NullContext::setMainWindow(void*)
    {
    }

    void NullContext::swapBuffers(void*)
    {
    }

    void NullContext::setTitle(void*, const char*)
    {
    }

    void NullContext::setFullScreen(void*, bool)
    {
    }

    Windowing::Window::PosT NullContext::getPosition(void*) const
    {
        return {};
    }

    void NullContext::setPosition(void*, Windowing::Window::PosT)
    {
    }

    Windowing::Window::DimensionsT NullContext::getSize(void*) const
    {
        return {};
    }

    void NullContext::setSize(void*, Windowing::Window::DimensionsT)
    {
    }

    void NullContext::setSizeLimits(void*, Windowing::Window::DimensionsT, Windowing::Window::DimensionsT)
    {
    }

    Windowing::Window::CursorPosT NullContext::getCursorPosition(void*) const
    {
        return {};
    }

    void NullContext::setCursorPosition(void*, Windowing::Window::CursorPosT)
    {
    }

    void NullContext::setCursorMode(void*, Core::ECursorMode)
    {
    }

    bool NullContext::shouldClose(void* handle) const
    {
        const auto it = m_shouldClose.find(reinterpret_cast<size_t>(handle));
        return it != m_shouldClose.end() && it->second;
    }

    void NullContext::setShouldClose(void* handle, bool shouldClose)
    {
        m_shouldClose[reinterpret_cast<size_t>(handle)] = shouldClose;
    }
}
