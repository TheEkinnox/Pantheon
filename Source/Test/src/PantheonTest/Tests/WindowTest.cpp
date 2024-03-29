#include "PantheonTest/Tests/WindowTest.h"

#include <PantheonApp/Windowing/Window.h>

#include <PantheonCore/Utility/ServiceLocator.h>

using namespace PantheonApp::Windowing;

namespace PantheonTest
{
    WindowTest::WindowTest()
        : WindowTest("Window")
    {
    }

    WindowTest::WindowTest(const std::string& name)
        : ITest(name), m_window(nullptr)
    {
    }

    void WindowTest::onStart()
    {
        m_window = &PTH_SERVICE(Window);

        testSize();
        testPosition();
        testFullscreen();
        testCursorPosition();
        testCursorMode();
        testExtra();

        complete();
    }

    void WindowTest::testExtra()
    {
        const std::string title = m_window->getTitle();
        const float aspect = m_window->getAspect();
        const int refreshRate = m_window->getRefreshRate();
        DEBUG_LOG("Window title: %s | Aspect: %f | Refresh rate: %d", title.c_str(), aspect, refreshRate);

        m_window->setTitle(title);
        TEST_CHECK(m_window->getTitle() == title, "Window title shouldn't have changed");

        const std::string newTitle("TEST - " + title);
        m_window->setTitle(newTitle);
        TEST_CHECK(m_window->getTitle() != title, "Window title should have changed");
        TEST_CHECK(m_window->getTitle() == newTitle, "Window title should match new title");

        m_window->setTitle(title);
        TEST_CHECK(m_window->getTitle() == title, "Window title should have been reset");

        m_window->setRefreshRate(refreshRate);
        TEST_CHECK(m_window->getRefreshRate() == refreshRate, "Window refresh rate shouldn't have changed");

        m_window->setRefreshRate(refreshRate + 5);
        TEST_CHECK(m_window->getRefreshRate() != refreshRate, "Window refresh rate should have changed");
        TEST_CHECK(m_window->getRefreshRate() == refreshRate + 5, "Window refresh rate should match new value");

        m_window->setRefreshRate(refreshRate);
        TEST_CHECK(m_window->getRefreshRate() == refreshRate, "Window refresh rate should have been reset");
    }

    void WindowTest::testSize()
    {
        const Window::DimensionsT windowSize = m_window->getSize();
        const Window::DimensionsT minSize = m_window->getMinSize();
        const Window::DimensionsT maxSize = m_window->getMaxSize();

        DEBUG_LOG("Window size: %s | Min size: %s | Max size: %s", windowSize.string().c_str(),
            minSize.string().c_str(), maxSize.string().c_str());

        TEST_CHECK(windowSize.m_x >= minSize.m_x && windowSize.m_y >= minSize.m_y,
            "Window size should be greater than or equal to min size");

        TEST_CHECK(windowSize.m_x <= maxSize.m_x && windowSize.m_y <= maxSize.m_y,
            "Window size should be smaller than or equal to max size");

        m_window->setSize(windowSize);
        TEST_CHECK(m_window->getSize() == windowSize, "Window size shouldn't have changed");

        Window::DimensionsT newWindowSize = windowSize * 1.5f;
        m_window->setSize(newWindowSize);
        TEST_CHECK(m_window->getSize() == clamp(newWindowSize, minSize, maxSize),
            "New window size should be between min and max size");

        const Window::DimensionsT newMinSize = newWindowSize * 1.5f;
        m_window->setMinSize(newMinSize);
        TEST_CHECK(m_window->getMinSize() == newMinSize, "Min window size should be updated");

        newWindowSize = m_window->getSize();
        TEST_CHECK(newWindowSize.m_x >= newMinSize.m_x && newWindowSize.m_y >= newMinSize.m_y,
            "Window size should be greather than or equal to new min size");

        const Window::DimensionsT newMaxSize = newWindowSize / 1.5f;
        m_window->setMaxSize(newMaxSize);
        TEST_CHECK(m_window->getMaxSize() == newMaxSize, "Max window size should be updated");

        newWindowSize = m_window->getSize();
        TEST_CHECK(newWindowSize.m_x <= newMaxSize.m_x && newWindowSize.m_y <= newMaxSize.m_y,
            "Window size should be smaller than or equal to new max size");

        m_window->setMinSize(minSize);
        m_window->setMaxSize(maxSize);
        m_window->setSize(windowSize);
        TEST_CHECK(m_window->getMinSize() == minSize, "Window min size should have been reset");
        TEST_CHECK(m_window->getMaxSize() == maxSize, "Window max size should have been reset");
        TEST_CHECK(m_window->getSize() == windowSize, "Window size should have been reset");
    }

    void WindowTest::testPosition()
    {
        const Window::PosT windowPos = m_window->getPosition();
        DEBUG_LOG("Window pos: %s", windowPos.string().c_str());

        m_window->setPosition(windowPos);
        TEST_CHECK(m_window->getPosition() == windowPos, "Window position shouldn't have changed");

        m_window->setPosition(windowPos + 5);
        TEST_CHECK(m_window->getPosition() != windowPos, "Window position should have changed");
        TEST_CHECK(m_window->getPosition() == windowPos + 5, "Window position should match the target value");

        m_window->setPosition(windowPos);
        TEST_CHECK(m_window->getPosition() == windowPos, "Window position should have been reset");
    }

    void WindowTest::testFullscreen()
    {
        const Window::PosT windowPos = m_window->getPosition();
        const bool isFullscreen = m_window->isFullScreen();
        DEBUG_LOG("Window fullscreen: %s", isFullscreen ? "on" : "off");

        m_window->setFullScreen(isFullscreen);
        TEST_CHECK(m_window->isFullScreen() == isFullscreen, "Window fullscreen mode shouldn't have changed");

        m_window->setFullScreen(!isFullscreen);
        TEST_CHECK(m_window->isFullScreen() != isFullscreen, "Window fullscreen mode should have changed");

        m_window->toggleFullScreen();
        TEST_CHECK(m_window->isFullScreen() == isFullscreen, "Window fullscreen mode should have been reset");

        m_window->setPosition(windowPos);
        TEST_CHECK(m_window->getPosition() == windowPos, "Window position should have been reset");
    }

    void WindowTest::testCursorPosition()
    {
        const Window::CursorPosT cursorPos = m_window->getCursorPosition();
        DEBUG_LOG("Cursor pos: %s", cursorPos.string().c_str());

        m_window->setCursorPosition(cursorPos);
        TEST_CHECK(m_window->getCursorPosition() == cursorPos, "Cursor position shouldn't have changed");

        m_window->setCursorPosition(cursorPos + 5);
        TEST_CHECK(m_window->getCursorPosition() != cursorPos, "Cursor position should have changed");
        TEST_CHECK(m_window->getCursorPosition() == cursorPos + 5, "Cursor position should match the target value");

        m_window->setCursorPosition(cursorPos);
        TEST_CHECK(m_window->getCursorPosition() == cursorPos, "Cursor position should have been reset");
    }

    void WindowTest::testCursorMode() const
    {
        m_window->setCursorPosition(m_window->getSize() * .5);
        m_window->showCursor();
        m_window->hideCursor();
        m_window->setCursorPosition({ m_window->getSize().m_x + 32., m_window->getSize().m_y * .5 });
        m_window->setCursorPosition(m_window->getSize() * .5);
        m_window->showCursor();
        m_window->disableCursor();
        m_window->setCursorPosition({ m_window->getSize().m_x + 256., m_window->getSize().m_y * .5 });
        m_window->showCursor();
    }
}
