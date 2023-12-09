#include "PantheonTest/Tests/ITest.h"

#include <PantheonCore/Debug/Logger.h>

namespace PantheonTest
{
    void ITest::start()
    {
        m_isDone = false;
        onStart();
    }

    void ITest::preUpdate()
    {
        if (!isDone())
            onPreUpdate();
    }

    void ITest::update()
    {
        if (!isDone())
            onUpdate();
    }

    void ITest::postUpdate()
    {
        if (!isDone())
            onPostUpdate();
    }

    void ITest::fixedUpdate()
    {
        if (!isDone())
            onFixedUpdate();
    }

    void ITest::stop()
    {
        if (isDone())
            return;

        onStop();
        m_isDone = true;
    }

    bool ITest::isDone() const
    {
        return m_isDone;
    }

    bool ITest::isSuccess() const
    {
        return m_isSuccess;
    }

    ITest::ITest() : ITest("Unnamed")
    {
    }

    ITest::ITest(std::string name)
        : m_executedCount(0), m_passedCount(0), m_failedCount(0), m_name(std::move(name)), m_isDone(false), m_isSuccess(false)
    {
    }

    void ITest::onStart()
    {
    }

    void ITest::onPreUpdate()
    {
    }

    void ITest::onUpdate()
    {
    }

    void ITest::onPostUpdate()
    {
    }

    void ITest::onFixedUpdate()
    {
    }

    void ITest::onStop()
    {
    }

    void ITest::complete()
    {
        m_isDone = true;
        m_isSuccess = m_passedCount == m_executedCount;

        if (m_isSuccess)
            DEBUG_LOG("= %s tests completed succesfully =", m_name.c_str());
        else
            DEBUG_LOG_ERROR("= %s tests failed =\n- %d executed\n- %d passed\n- %d failed",
            m_name.c_str(), m_executedCount, m_passedCount, m_failedCount);
    }
}
