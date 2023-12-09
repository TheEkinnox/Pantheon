#pragma once
#include "PantheonTest/Tests/ITest.h"

namespace PantheonEngine::Application::Windowing
{
    class Window;
}

namespace PantheonTest
{
    class WindowTest final : public ITest
    {
    public:
        WindowTest();
        explicit WindowTest(const std::string& name);

        void onStart() override;

    private:
        PantheonEngine::Application::Windowing::Window* m_window;

        void testSize();
        void testPosition();
        void testFullscreen();
        void testCursorPosition();
        void testCursorMode() const;
        void testExtra();
    };
}
