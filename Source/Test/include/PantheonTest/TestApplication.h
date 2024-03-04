#pragma once
#include "PantheonTest/Tests/ITest.h"

#include <PantheonApp/Core/IApplication.h>
#include <PantheonApp/Input/InputManager.h>
#include <PantheonApp/Windowing/Window.h>

#include <PantheonCore/Resources/ResourceManager.h>
#include <PantheonCore/Utility/ThreadPool.h>

namespace PantheonTest
{
    class TestApplication final : public PantheonApp::Core::IApplication
    {
    public:
        TestApplication();

    protected:
        void onStart(int argc, char* argv[]) override;
        void preUpdate();
        void update();
        void postUpdate();
        void onUpdate(float deltaTime) override;
        void onFixedUpdate(float deltaTime) override;
        void onStop() override;

    public:
        bool isRunning() const override;

    private:
        std::unique_ptr<PantheonApp::Windowing::Window>           m_window;
        std::unique_ptr<PantheonApp::Input::InputManager>         m_inputManager;
        std::unique_ptr<PantheonCore::Utility::ThreadPool>        m_threadPool;
        std::unique_ptr<PantheonCore::Resources::ResourceManager> m_resourceManager;

        std::vector<std::shared_ptr<ITest>> m_tests;

        std::chrono::high_resolution_clock::time_point m_startTime;
    };
}
