#pragma once
#include "PantheonApp/Core/IApplication.h"
#include "PantheonApp/Input/InputManager.h"
#include "PantheonApp/Windowing/Window.h"

namespace PantheonEngine::Test
{
    class TestApplication final : public Application::Core::IApplication
    {
    public:
        TestApplication();

    protected:
        void onStart(int argc, char* argv[]) override;
        void preUpdate();
        void update();
        void postUpdate();
        void onUpdate() override;
        void onFixedUpdate() override;
        void onStop() override;

    public:
        bool isRunning() const override;

    private:
        std::unique_ptr<Application::Windowing::Window> m_window;
        std::unique_ptr<Application::Input::InputManager> m_inputManager;
    };
}
