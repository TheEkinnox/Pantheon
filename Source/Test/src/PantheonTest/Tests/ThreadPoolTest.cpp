#include "PantheonTest/Tests/ThreadPoolTest.h"

#include "PantheonCore/Utility/ServiceLocator.h"

using namespace PantheonCore::Utility;

namespace PantheonTest
{
    ThreadPoolTest::ThreadPoolTest(const size_t taskCount, const size_t taskDuration)
        : ThreadPoolTest("Thread Pool", taskCount, taskDuration)
    {
    }

    ThreadPoolTest::ThreadPoolTest(const std::string& name, const size_t taskCount, const size_t taskDuration)
        : ITest(name), m_threadPool(&PTH_SERVICE(ThreadPool)), m_taskCount(taskCount), m_taskDuration(taskDuration)
    {
    }

    void ThreadPoolTest::onStart()
    {
        DEBUG_LOG("Testing thread pool - Executing %llu tasks of %llums", m_taskCount, m_taskDuration);

        auto start = std::chrono::high_resolution_clock::now();

        for (size_t i = 0; i < m_taskCount; ++i)
            std::this_thread::sleep_for(std::chrono::milliseconds(m_taskDuration));

        auto end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("Singe thread: %dms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

        start = std::chrono::high_resolution_clock::now();
        std::vector<std::future<void>> tasks;
        tasks.reserve(m_taskCount);

        for (size_t i = 0; i < m_taskCount; ++i)
        {
            tasks.emplace_back(m_threadPool->enqueue([taskDuration = m_taskDuration]
            {
                std::this_thread::sleep_for(std::chrono::milliseconds(taskDuration));
            }));
        }

        TEST_CHECK(tasks.size() == m_taskCount, "ThreadPool tasks count should be %llu - Received %llu", m_taskCount, tasks.size());

        for (auto& task : tasks)
            task.wait();

        end = std::chrono::high_resolution_clock::now();
        DEBUG_LOG("Multi thread: %dms", std::chrono::duration_cast<std::chrono::milliseconds>(end - start));

        complete();
    }
}
