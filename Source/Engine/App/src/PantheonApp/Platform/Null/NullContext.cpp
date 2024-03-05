#include "PantheonApp/Platform/Null/NullContext.h"

#include <PantheonCore/Debug/Assertion.h>

#include <GLFW/glfw3.h>

namespace PantheonApp::Platform
{
    NullContext::NullContext(const bool, int)
        : m_windowHandle(nullptr)
    {
        // Initialize and configure glfw
        [[maybe_unused]] const auto result = glfwInit();
        ASSERT(result == GLFW_TRUE, "Failed to initialize GLFW");

        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);

        m_isInitialized = true;
    }

    bool NullContext::isInitialized() const
    {
        return m_isInitialized;
    }

    bool NullContext::getVsync() const
    {
        return false;
    }

    void NullContext::setVsync(const bool)
    {
    }

    void NullContext::setMainWindow(void* handle)
    {
        m_windowHandle = static_cast<GLFWwindow*>(handle);
    }

    void NullContext::swapBuffers()
    {
    }

    void NullContext::update()
    {
        IContext::update();
    }
}
