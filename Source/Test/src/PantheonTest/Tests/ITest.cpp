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

    ITest::ITest() : ITest("Unnamed")
    {
    }

    ITest::ITest(std::string name) : m_name(std::move(name)), m_isDone(false)
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
        DEBUG_LOG("= %s tests completed succesfully =", m_name.c_str());
    }
}
