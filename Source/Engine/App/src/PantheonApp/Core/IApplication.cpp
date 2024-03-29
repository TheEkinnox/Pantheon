#include "PantheonApp/Core/IApplication.h"

#include <PantheonCore/Debug/Assertion.h>

using namespace PantheonCore::Utility;

namespace PantheonApp::Core
{
    void IApplication::run(const int argc, char* argv[])
    {
        ASSERT(m_context && m_context->isInitialized(), "Unable to run application - context is undefined");

        Timer& timer = m_context->m_timer;
        timer.reset();

        onStart(argc, argv);

        const auto updateListenerId = timer.m_onUpdate.subscribe([this](const float deltaTime)
        {
            onUpdate(deltaTime);
        });

        const auto fixedUpdateListenerId = timer.m_onFixedUpdate.subscribe([this](const float deltaTime)
        {
            onFixedUpdate(deltaTime);
        });

        while (isRunning())
            m_context->update();

        timer.m_onUpdate.unsubscribe(updateListenerId);
        timer.m_onFixedUpdate.unsubscribe(fixedUpdateListenerId);

        onStop();
    }

    IApplication::IApplication(std::unique_ptr<IContext> context)
        : m_context(std::move(context))
    {
    }

    IContext& IApplication::getContext() const
    {
        ASSERT(m_context);
        return *m_context;
    }
}
