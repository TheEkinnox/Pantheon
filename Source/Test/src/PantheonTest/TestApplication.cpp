#include "PantheonTest/TestApplication.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

using namespace PantheonEngine::Core::Utility;
using namespace PantheonEngine::Application::Core;
using namespace PantheonEngine::Application::Input;
using namespace PantheonEngine::Application::Windowing;

namespace PantheonTest
{
    TestApplication::TestApplication() :
        IApplication(std::make_unique<Context>(true, 4)),
        m_window(std::make_unique<Window>(getContext(), WindowSettings{ "Pantheon Test", 800, 600 })),
        m_inputManager(std::make_unique<InputManager>(*m_window))
    {
        ServiceLocator::provide<Window>(*m_window);
        ServiceLocator::provide<InputManager>(*m_inputManager);
    }

    void TestApplication::onStart([[maybe_unused]] int argc, [[maybe_unused]] char* argv[])
    {
        m_window->makeCurrentContext();
    }

    void TestApplication::preUpdate()
    {
        if (m_inputManager->isKeyPressed(EKey::KEY_ESCAPE))
            m_window->setShouldClose(true);
    }

    int timeSinceLastSpace = 0;

    void TestApplication::update()
    {
        if (m_inputManager->isKeyDown(EKey::KEY_P))
            std::cout << "A";

        if (m_inputManager->isKeyReleased(EKey::KEY_P))
            std::cout << "H\n" << std::endl;

        if (m_inputManager->isKeyUp(EKey::KEY_SPACE))
        {
            timeSinceLastSpace++;
        }
        else if (m_inputManager->isKeyPressed(EKey::KEY_SPACE))
        {
            DEBUG_LOG("Time since last space : %i", timeSinceLastSpace);
            timeSinceLastSpace = 0;
        }
    }

    void TestApplication::postUpdate()
    {
        m_window->swapBuffers();
        m_inputManager->update();
    }

    void TestApplication::onUpdate()
    {
        preUpdate();
        update();
        postUpdate();
    }

    void TestApplication::onFixedUpdate()
    {
    }

    void TestApplication::onStop()
    {
    }

    bool TestApplication::isRunning() const
    {
        return !m_window->shouldClose();
    }
}
