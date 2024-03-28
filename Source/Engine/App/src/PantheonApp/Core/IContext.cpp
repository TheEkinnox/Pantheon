#include "PantheonApp/Core/IContext.h"

#include "PantheonApp/Platform/Null/NullContext.h"
#include "PantheonApp/Platform/OpenGL/OpenGLContext.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include <PantheonRendering/RHI/IRenderAPI.h>

#include <GLFW/glfw3.h>

using namespace PantheonCore::Debug;
using namespace PantheonCore::Utility;
using namespace PantheonApp::Platform;
using namespace PantheonRendering::RHI;

namespace PantheonApp::Core
{
    void IContext::update()
    {
        m_timer.tick();
    }

    std::unique_ptr<IContext> IContext::create(const EGraphicsAPI backend, bool useVsync, int sampleCount)
    {
        IRenderAPI::setCurrent(backend);

        switch (backend)
        {
        case EGraphicsAPI::NONE:
            return std::make_unique<NullContext>(useVsync, sampleCount);
        case EGraphicsAPI::OPENGL:
            return std::make_unique<OpenGLContext>(useVsync, sampleCount);
        default:
            ASSERT(false, "Unable to create context - Unsupported backend");
            return {};
        }
    }

    IContext::IContext()
    {
        ServiceLocator::provide<Timer>(m_timer);
    }
}
