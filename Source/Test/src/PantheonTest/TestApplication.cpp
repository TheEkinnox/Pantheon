#include "PantheonTest/TestApplication.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/FileSystem.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include "PantheonTest/Tests/InputTest.h"
#include "PantheonTest/Tests/ThreadPoolTest.h"
#include "PantheonTest/Tests/WindowTest.h"

using namespace PantheonCore::Utility;
using namespace PantheonCore::Resources;
using namespace PantheonApp::Core;
using namespace PantheonApp::Input;
using namespace PantheonApp::Windowing;

namespace PantheonTest
{
    TestApplication::TestApplication() :
        IApplication(std::make_unique<Context>(true, 4)),
        m_window(std::make_unique<Window>(getContext(), WindowSettings{ "Pantheon Test", 800, 600 })),
        m_inputManager(std::make_unique<InputManager>(*m_window)),
        m_threadPool(std::make_unique<ThreadPool>()),
        m_resourceManager(std::make_unique<ResourceManager>()),
        m_startTime(std::chrono::high_resolution_clock::now())
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
        const char* appDir = getApplicationDirectory();
        std::string workingDir = getWorkingDirectory();

        DEBUG_LOG("Changing working directory from \"%s\" to \"%s\"", workingDir.c_str(), appDir);
        changeDirectory(appDir);

        workingDir = getWorkingDirectory();
        DEBUG_LOG("Current working directory: \"%s\"", workingDir.c_str());
        ASSERT(workingDir == appDir, "Invalid working directory - Expected: \"%s\"", appDir);

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

        const auto      endTime = std::chrono::high_resolution_clock::now();
        const long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();

        if (passedCount == m_tests.size())
        {
            DEBUG_LOG("All %llu tests passed | Total execution time: %dms", passedCount, elapsedTime);
        }
        else
        {
            DEBUG_LOG_ERROR("%llu/%llu Tests passed | Total execution time: %dms", passedCount, m_tests.size(), elapsedTime);
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
