#pragma once
#include "PantheonApp/Core/IContext.h"

namespace PantheonApp::Platform
{
    class NullContext final : public Core::IContext
    {
    public:
        NullContext(bool useVsync, int sampleCount, int refreshRate);
        ~NullContext() override = default;

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
         * \brief Gets the context's refresh rate
         * \return The context's refresh rate
         */
        int getRefreshRate() const override;

        /**
         * \brief Sets the context's refresh rate
         * \param refreshRate The context's new refresh rate
         */
        void setRefreshRate(int refreshRate) override;

        /**
         * \brief Gets the context's current main windows
         * \return The context's current main window
         */
        Windowing::Window* getMainWindow() const override;

        /**
         * \brief Updates the context's data
         */
        void update() override;

    private:
        size_t m_mainWindow;
        bool   m_isInitialized;

        std::unordered_map<size_t, bool> m_shouldClose;

        /**
         * \brief Creates a window handle with the given settings
         * \param settings The window creation settings
         * \param window The created handle's owner
         */
        void* createWindowHandle(const Windowing::WindowSettings& settings, Windowing::Window* window) override;

        /**
         * \brief Destroys the given window handle
         * \param handle The window handle to destroy
         */
        void destroyWindowHandle(void*& handle) override;

        /**
         * \brief Sets the context's main window
         * \param handle The target window handle
         */
        void setMainWindow(void* handle) override;

        /**
         * \brief Swaps the given window's render buffers
         */
        void swapBuffers(void* handle) override;

        /**
         * \brief Sets the target window's fullscreen mode
         * \param handle The target window's handle
         */
        void setTitle(void* handle, const char* title) override;

        /**
         * \brief Sets the target window's fullscreen mode
         * \param handle The target window's handle
         * \param shouldEnable The target window's new fullscreen mode
         */
        void setFullScreen(void* handle, bool shouldEnable) override;

        /**
         * \brief Gets the target window's position
         * \param handle The target window's handle
         * \return The target window's position
         */
        Windowing::Window::PosT getPosition(void* handle) const override;

        /**
         * \brief Sets the target window's position
         * \param handle The target window's handle
         * \param position The target window's new position
         */
        void setPosition(void* handle, Windowing::Window::PosT position) override;

        /**
         * \brief Gets the target window's size
         * \param handle The target window's handle
         * \return The target window's size
         */
        Windowing::Window::DimensionsT getSize(void* handle) const override;

        /**
         * \brief Sets the target window's size
         * \param handle The target window's handle
         * \param size The target window's new size
         */
        void setSize(void* handle, Windowing::Window::DimensionsT size) override;

        /**
         * \brief Sets the target window's size
         * \param handle The target window's handle
         * \param min The target window's new min size
         * \param max The target window's new max size
         */
        void setSizeLimits(void* handle, Windowing::Window::DimensionsT min, Windowing::Window::DimensionsT max) override;

        /**
         * \brief Gets the target window's cursor position
         * \param handle The target window's handle
         * \return The target window's cursor position
         */
        Windowing::Window::CursorPosT getCursorPosition(void* handle) const override;

        /**
         * \brief Sets the target window's cursor position
         * \param handle The target window's handle
         * \param pos The target window's new cursor position
         */
        void setCursorPosition(void* handle, Windowing::Window::CursorPosT pos) override;

        /**
         * \brief Sets the target window's cursor position
         * \param handle The target window's handle
         * \param mode The target window's new cursor mode
         */
        void setCursorMode(void* handle, Core::ECursorMode mode) override;

        /**
         * \brief Whether the window should close or not
         * \param handle The target window's handle
         * \return True if the window should close. False otherwise
         */
        bool shouldClose(void* handle) const override;

        /**
         * \brief Sets whether the window should close or not
         * \param handle The target window's handle
         * \param shouldClose Whether the window should close or not
         */
        void setShouldClose(void* handle, bool shouldClose) override;
    };
}
