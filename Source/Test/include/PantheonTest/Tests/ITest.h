#pragma once
#include <PantheonCore/Debug/Assertion.h>
#include <PantheonCore/Utility/macros.h>

#include <string>

#define TEST_CHECK(...) ++m_executedCount; CHECK(__VA_ARGS__) ? ++m_passedCount : ++m_failedCount
#define TEST_REQUIRE(condition, ...) ++m_executedCount; const bool UNIQUE_VAR(result) = CHECK(condition, __VA_ARGS__); \
    UNIQUE_VAR(result) ? ++m_passedCount : ++m_failedCount; if (!UNIQUE_VAR(result)) return

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
