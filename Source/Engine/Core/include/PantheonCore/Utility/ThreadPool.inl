#pragma once
#include "ThreadPool.h"

namespace PantheonEngine::Core::Utility
{
    template <typename Func, typename... Args>
    std::future<std::invoke_result_t<Func, Args...>> ThreadPool::enqueue(Func&& func, Args&&... args)
    {
        using PackagedTask = std::packaged_task<std::invoke_result_t<Func, Args...>()>;
        auto package = std::make_shared<PackagedTask>(std::bind(std::forward<Func>(func), std::forward<Args>(args)...));

        {
            std::lock_guard lock(m_tasksMutex);
            m_tasks.emplace([package]
            {
                (*package)();
            });
        }

        m_mutexCondition.notify_one();
        return package->get_future();
    }
}
