#pragma once
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <type_traits>

namespace PantheonCore::Utility
{
    class ThreadPool
    {
    public:
        using Action = std::function<void()>;

        ThreadPool();
        explicit ThreadPool(unsigned workersCount);
        ThreadPool(const ThreadPool&) = delete;
        ThreadPool(ThreadPool&&) = delete;

        ThreadPool& operator=(const ThreadPool&) = delete;
        ThreadPool& operator=(ThreadPool&&) = delete;

        ~ThreadPool();

        void start();

        template <typename Func, typename... Args>
        std::future<std::invoke_result_t<Func, Args...>> enqueue(Func&& func, Args&&... args);

        bool isBusy() const;
        void stop();

        unsigned getWorkersCount() const;
        unsigned getActiveCount() const;

    private:
        mutable std::mutex       m_tasksMutex;
        std::condition_variable  m_mutexCondition;
        std::queue<Action>       m_tasks;
        std::vector<std::thread> m_threads;
        unsigned                 m_workersCount;
        unsigned                 m_activeWorkersCount;
        bool                     m_isRunning;
        bool                     m_shouldTerminate;

        void workerLoop();
    };
}

#include "ThreadPool.inl"
