#pragma once
#include <PantheonCore/Debug/Assertion.h>

#include <string>

#define TEST_CHECK(...) ++m_executedCount; CHECK(__VA_ARGS__) ? ++m_passedCount : ++m_failedCount

namespace PantheonTest
{
    class ITest
    {
    public:
        virtual ~ITest() = default;

        void start();

        void preUpdate();

        void update();

        void postUpdate();

        void fixedUpdate();

        void stop();

        bool isDone() const;

        bool isSuccess() const;

    protected:
        int m_executedCount;
        int m_passedCount;
        int m_failedCount;

        explicit ITest(std::string name);

        virtual void onStart();

        virtual void onPreUpdate();

        virtual void onUpdate();

        virtual void onPostUpdate();

        virtual void onFixedUpdate();

        virtual void onStop();

        void complete();

    private:
        std::string m_name;
        bool        m_isDone;
        bool        m_isSuccess;
    };
}
