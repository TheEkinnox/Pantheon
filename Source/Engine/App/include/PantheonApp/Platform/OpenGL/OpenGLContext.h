#pragma once
#include "PantheonApp/Core/IContext.h"

struct GLFWwindow;

namespace PantheonApp::Platform
{
    class OpenGLContext final : public Core::IContext
    {
        static constexpr unsigned int PANTHEON_OPENGL_VERSION_MAJOR = 4;
        static constexpr unsigned int PANTHEON_OPENGL_VERSION_MINOR = 6;

    public:
        OpenGLContext(bool useVsync, int sampleCount);
        ~OpenGLContext() override;

        /**
         * \brief Checks whether the current application context has been initialized
         * \return True if the context is initialized, false otherwise.
         */
        bool isInitialized() const override;

        /**
         * \brief Checks whether vsync is currently enabled or not
         * \return True if vsync is enabled, false otherwise.
         */
        bool getVsync() const override;

        /**
         * \brief Enables or disables vertical synchronisation
         * \param useVsync Whether vsync should be enabled or not
         */
        void setVsync(bool useVsync) override;

        /**
         * \brief Sets the context's main window
         * \param handle The target window handle
         */
        void setMainWindow(void* handle) override;

        /**
         * \brief Swaps the render buffers
         */
        void swapBuffers() override;

        /**
         * \brief Updates the context's data
         */
        void update() override;

    private:
        GLFWwindow* m_windowHandle;
        bool        m_useVsync;
        bool        m_isInitialized;
    };
}
