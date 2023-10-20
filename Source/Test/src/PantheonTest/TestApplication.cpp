#include "PantheonTest/TestApplication.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include "PantheonTest/Tests/ThreadPoolTest.h"
#include "PantheonTest/Tests/WindowTest.h"

using namespace PantheonEngine::Core::Utility;
using namespace PantheonEngine::Application::Core;
using namespace PantheonEngine::Application::Input;
using namespace PantheonEngine::Application::Windowing;

namespace PantheonTest
{
    TestApplication::TestApplication() :
        IApplication(std::make_unique<Context>(true, 4)),
        m_window(std::make_unique<Window>(getContext(), WindowSettings{ "Pantheon Test", 800, 600 })),
        m_inputManager(std::make_unique<InputManager>(*m_window)),
        m_threadPool(std::make_unique<ThreadPool>())
    {
        ServiceLocator::provide<Window>(*m_window);
        ServiceLocator::provide<InputManager>(*m_inputManager);
        ServiceLocator::provide<ThreadPool>(*m_threadPool);
    }

    void TestApplication::onStart(int, char*[])
    {
        m_window->makeCurrentContext();
        WindowTest().run();
        ThreadPoolTest().run();
    }

    void TestApplication::preUpdate()
    {
        if (m_inputManager->isKeyPressed(EKey::KEY_ESCAPE))
            m_window->setShouldClose(true);

        if (m_inputManager->isKeyPressed(EKey::KEY_F11))
            m_window->toggleFullScreen();
    }

    int timeSinceLastSpace = 0;

    void TestApplication::update()
    {
        const bool isShiftDown = m_inputManager->isKeyDown(EKey::KEY_LEFT_SHIFT)
            || m_inputManager->isKeyDown(EKey::KEY_RIGHT_SHIFT);

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

        if (m_inputManager->isKeyPressed(EKey::KEY_C))
            m_window->setCursorPosition(m_window->getSize() / 2);

        if (m_inputManager->isKeyPressed(EKey::KEY_H))
        {
            if (isShiftDown)
                m_window->showCursor();
            else
                m_window->hideCursor();
        }

        if (m_inputManager->isKeyPressed(EKey::KEY_X))
        {
            if (isShiftDown)
                m_window->showCursor();
            else
                m_window->disableCursor();
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
