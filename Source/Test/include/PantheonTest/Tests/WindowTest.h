#pragma once
#include "PantheonTest/Tests/ITest.h"

namespace PantheonApp::Windowing
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
        PantheonApp::Windowing::Window* m_window;

        void testSize();
        void testPosition();
        void testFullscreen();
        void testCursorPosition();
        void testCursorMode() const;
        void testExtra();
    };
}
