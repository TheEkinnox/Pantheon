#include "PantheonCore/Utility/ThreadPool.h"

namespace PantheonEngine::Core::Utility
{
    ThreadPool::ThreadPool()
        : ThreadPool(std::thread::hardware_concurrency() - 1)
    {
    }

    ThreadPool::ThreadPool(const unsigned workersCount)
        : m_workersCount(workersCount), m_activeWorkersCount(0), m_isRunning(false), m_shouldTerminate(false)
    {
        start();
    }

    ThreadPool::~ThreadPool()
    {
        stop();
    }

    void ThreadPool::start()
    {
        if (m_isRunning)
            return;

        m_threads.reserve(m_workersCount);

        for (unsigned i = 0; i < m_workersCount; ++i)
            m_threads.emplace_back(&ThreadPool::workerLoop, this);

        m_isRunning = true;
    }

    bool ThreadPool::isBusy() const
    {
        {
            std::lock_guard lock(m_tasksMutex);
            return !m_tasks.empty() || m_activeWorkersCount > 0;
        }
    }

    void ThreadPool::stop()
    {
        if (!m_isRunning)
            return;

        {
            std::lock_guard lock(m_tasksMutex);
            m_shouldTerminate = true;
        }

        m_mutexCondition.notify_all();

        for (std::thread& thread : m_threads)
            thread.join();

        m_threads.clear();
        m_isRunning = false;
    }

    unsigned ThreadPool::getWorkersCount() const
    {
        return m_workersCount;
    }

    unsigned ThreadPool::getActiveCount() const
    {
        std::lock_guard lock(m_tasksMutex);
        return m_activeWorkersCount;
    }

    void ThreadPool::workerLoop()
    {
        while (true)
        {
            Action task;

            {
                std::unique_lock lock(m_tasksMutex);
                m_mutexCondition.wait(lock, [this]
                    {
                        return !m_tasks.empty() || m_shouldTerminate;
                    }
                );

                if (m_shouldTerminate)
                    return;

                ++m_activeWorkersCount;
                task = std::move(m_tasks.front());
                m_tasks.pop();
            }

            task();

            {
                std::lock_guard lock(m_tasksMutex);
                --m_activeWorkersCount;
            }
        }
    }
}
