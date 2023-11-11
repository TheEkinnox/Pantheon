#pragma once
#include <PantheonApp/Core/IApplication.h>
#include <PantheonApp/Input/InputManager.h>
#include <PantheonApp/Windowing/Window.h>

#include <PantheonCore/Utility/ThreadPool.h>

#include "PantheonTest/Tests/ITest.h"

namespace PantheonTest
{
    class TestApplication final : public PantheonEngine::Application::Core::IApplication
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
        std::unique_ptr<PantheonEngine::Application::Windowing::Window>   m_window;
        std::unique_ptr<PantheonEngine::Application::Input::InputManager> m_inputManager;
        std::unique_ptr<PantheonEngine::Core::Utility::ThreadPool>        m_threadPool;

        std::vector<std::shared_ptr<ITest>> m_tests;
    };
}
