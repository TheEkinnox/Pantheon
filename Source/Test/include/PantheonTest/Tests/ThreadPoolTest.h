#pragma once
#include "ITest.h"

#include <PantheonCore/Utility/ThreadPool.h>

namespace PantheonTest
{
    class ThreadPoolTest : public ITest
    {
    public:
        explicit ThreadPoolTest(size_t taskCount = 50, size_t taskDuration = 200);

        void run() override;

    private:
        PantheonEngine::Core::Utility::ThreadPool* m_threadPool;

        size_t m_taskCount;
        size_t m_taskDuration;
    };
}