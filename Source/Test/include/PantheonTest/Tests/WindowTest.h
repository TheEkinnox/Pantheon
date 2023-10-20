#pragma once
#include "PantheonTest/Tests/ITest.h"

namespace PantheonEngine::Application::Windowing
{
    class Window;
}

namespace PantheonTest
{
    class WindowTest : public ITest
    {
    public:
        WindowTest();
        void run() override;

    private:
        PantheonEngine::Application::Windowing::Window* m_window;

        void testSize() const;
        void testPosition() const;
        void testFullscreen() const;
        void testCursorPosition() const;
        void testCursorMode() const;
        void testExtra() const;
    };
}
