#include "PantheonApp/Core/IApplication.h"

#include "PantheonCore/Debug/Assertion.h"

using namespace PantheonCore::Utility;

namespace PantheonApp::Core
{
    void IApplication::run(const int argc, char* argv[])
    {
        if (!m_context && !m_context->isInitialized())
            throw std::runtime_error("Unable to run application - context is undefined");

        Timer& timer = *getContext().m_timer;
        timer.reset();

        onStart(argc, argv);

        const auto updateListenerId = timer.m_onUpdate.subscribe([this]
        {
            onUpdate();
        });

        const auto fixedUpdateListenerId = timer.m_onFixedUpdate.subscribe([this]
        {
            onFixedUpdate();
        });

        while (isRunning())
            m_context->update();

        timer.m_onUpdate.unsubscribe(updateListenerId);
        timer.m_onFixedUpdate.unsubscribe(fixedUpdateListenerId);

        onStop();
    }

    IApplication::IApplication(std::unique_ptr<Context> context) :
        m_context(std::move(context))
    {
    }

    const Context& IApplication::getContext() const
    {
        ASSERT(m_context);
        return *m_context;
    }
}
