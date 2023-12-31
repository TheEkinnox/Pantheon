﻿#include "PantheonTest/TestApplication.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include "PantheonTest/Tests/InputTest.h"
#include "PantheonTest/Tests/ThreadPoolTest.h"
#include "PantheonTest/Tests/WindowTest.h"

using namespace PantheonEngine::Core::Utility;
using namespace PantheonEngine::Core::Resources;
using namespace PantheonEngine::Application::Core;
using namespace PantheonEngine::Application::Input;
using namespace PantheonEngine::Application::Windowing;

namespace PantheonTest
{
    TestApplication::TestApplication() :
        IApplication(std::make_unique<Context>(true, 4)),
        m_window(std::make_unique<Window>(getContext(), WindowSettings{ "Pantheon Test", 800, 600 })),
        m_inputManager(std::make_unique<InputManager>(*m_window)),
        m_threadPool(std::make_unique<ThreadPool>()),
        m_resourceManager(std::make_unique<ResourceManager>())
    {
        ServiceLocator::provide<Window>(*m_window);
        ServiceLocator::provide<InputManager>(*m_inputManager);
        ServiceLocator::provide<ThreadPool>(*m_threadPool);
        ServiceLocator::provide<ResourceManager>(*m_resourceManager);

        m_tests.push_back(std::make_shared<WindowTest>());
        m_tests.push_back(std::make_shared<InputTest>());
        m_tests.push_back(std::make_shared<ThreadPoolTest>());
    }

    void TestApplication::onStart(int, char*[])
    {
        m_window->makeCurrentContext();

        for (const auto& test : m_tests)
            test->start();
    }

    void TestApplication::preUpdate()
    {
        for (const auto& test : m_tests)
            test->preUpdate();
    }

    void TestApplication::update()
    {
        for (const auto& test : m_tests)
            test->update();
    }

    void TestApplication::postUpdate()
    {
        for (const auto& test : m_tests)
            test->postUpdate();

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
        for (const auto& test : m_tests)
            test->fixedUpdate();
    }

    void TestApplication::onStop()
    {
        size_t passedCount = 0;
        for (const auto& test : m_tests)
        {
            test->stop();

            if (test->isSuccess())
                ++passedCount;
        }

        if (passedCount == m_tests.size())
        {
            DEBUG_LOG("All %llu tests passed", passedCount);
        }
        else
        {
            DEBUG_LOG_ERROR("%llu/%llu Tests passed", passedCount, m_tests.size());
            std::quick_exit(-1);
        }
    }

    bool TestApplication::isRunning() const
    {
        const auto isInProgress = [](const std::shared_ptr<ITest>& test)
        {
            return !test->isDone();
        };

        return !m_window->shouldClose() && std::ranges::find_if(m_tests, isInProgress) != m_tests.end();
    }
}
