#pragma once
#include "PantheonApp/Core/ECursorMode.h"
#include "PantheonApp/Windowing/Window.h"
#include "PantheonApp/Windowing/WindowSettings.h"

#include "PantheonCore/Utility/Timer.h"

#include "PantheonRendering/RHI/EGraphicsAPI.h"

#include <memory>

namespace PantheonApp::Core
{
    class IContext
    {
    public:
        PantheonCore::Utility::Timer m_timer;

        /**
         * \brief Disable copy constructor
         */
        IContext(const IContext& other) = delete;

        /**
         * \brief Creates a move copy of the given application context
         * \param other The application context to move
         */
        IContext(IContext&& other) = default;

        /**
         * \brief Clears the context's previously allocated resources
         */
        virtual ~IContext() = default;

        /**
         * \brief Disable copy assignment operator
         */
        IContext& operator=(const IContext& other) = delete;

        /**
         * \brief Creates a move copy of the given application context
         * \param other The application context to move
         */
        IContext& operator=(IContext&& other) = default;

        /**
         * \brief Checks whether the current application context has been initialized
         * \return True if the context is initialized, false otherwise.
         */
        virtual bool isInitialized() const = 0;

        /**
         * \brief Checks whether vsync is currently enabled or not
         * \return True if vsync is enabled, false otherwise.
         */
        virtual bool getVsync() const = 0;

        /**
         * \brief Enables or disables vertical synchronisation
         * \param useVsync Whether vsync should be enabled or not
         */
        virtual void setVsync(bool useVsync) = 0;

        /**
         * \brief Gets the context's refresh rate
         * \return The context's refresh rate
         */
        virtual int getRefreshRate() const = 0;

        /**
         * \brief Sets the context's refresh rate
         * \param refreshRate The context's new refresh rate
         */
        virtual void setRefreshRate(int refreshRate) = 0;

        /**
         * \brief Gets the context's current main windows
         * \return The context's current main window
         */
        virtual Windowing::Window* getMainWindow() const = 0;

        /**
         * \brief Updates the context's data
         */
        virtual void update();

        /**
         * \brief Creates an application context for the given graphics api with the given settings
         * \param backend The target graphics api
         * \param useVsync Whether vsync should be enabled or not
         * \param sampleCount The number of samples to use
         * \param refreshRate The target refresh rate
         * \return The created application context
         */
        static std::unique_ptr<IContext> create(PantheonRendering::RHI::EGraphicsAPI backend, bool useVsync, int sampleCount,
                                                int refreshRate = Windowing::WindowSettings::DONT_CARE);

    protected:
        friend class Windowing::Window;

        IContext();

        /**
         * \brief Creates a window handle with the given settings
         * \param settings The window creation settings
         * \param window The created handle's owner
         */
        virtual void* createWindowHandle(const Windowing::WindowSettings& settings, Windowing::Window* window) = 0;

        /**
         * \brief Destroys the given window handle
         * \param handle The window handle to destroy
         */
        virtual void destroyWindowHandle(void*& handle) = 0;

        /**
         * \brief Sets the context's main window
         * \param handle The target window handle
         */
        virtual void setMainWindow(void* handle) = 0;

        /**
         * \brief Swaps the given window's render buffers
         */
        virtual void swapBuffers(void* handle) = 0;

        /**
         * \brief Sets the target window's title
         * \param handle The target window's handle
         * \param title The window's new title
         */
        virtual void setTitle(void* handle, const char* title) = 0;

        /**
         * \brief Sets the target window's fullscreen mode
         * \param handle The target window's handle
         * \param shouldEnable The target window's new fullscreen mode
         */
        virtual void setFullScreen(void* handle, bool shouldEnable) = 0;

        /**
         * \brief Gets the target window's position
         * \param handle The target window's handle
         * \return The target window's position
         */
        virtual Windowing::Window::PosT getPosition(void* handle) const = 0;

        /**
         * \brief Sets the target window's position
         * \param handle The target window's handle
         * \param position The target window's new position
         */
        virtual void setPosition(void* handle, Windowing::Window::PosT position) = 0;

        /**
         * \brief Gets the target window's size
         * \param handle The target window's handle
         * \return The target window's size
         */
        virtual Windowing::Window::DimensionsT getSize(void* handle) const = 0;

        /**
         * \brief Sets the target window's size
         * \param handle The target window's handle
         * \param size The target window's new size
         */
        virtual void setSize(void* handle, Windowing::Window::DimensionsT size) = 0;

        /**
         * \brief Sets the target window's size
         * \param handle The target window's handle
         * \param min The target window's new min size
         * \param max The target window's new max size
         */
        virtual void setSizeLimits(void* handle, Windowing::Window::DimensionsT min, Windowing::Window::DimensionsT max) = 0;

        /**
         * \brief Gets the target window's cursor position
         * \param handle The target window's handle
         * \return The target window's cursor position
         */
        virtual Windowing::Window::CursorPosT getCursorPosition(void* handle) const = 0;

        /**
         * \brief Sets the target window's cursor position
         * \param handle The target window's handle
         * \param pos The target window's new cursor position
         */
        virtual void setCursorPosition(void* handle, Windowing::Window::CursorPosT pos) = 0;

        /**
         * \brief Sets the target window's cursor position
         * \param handle The target window's handle
         * \param mode The target window's new cursor mode
         */
        virtual void setCursorMode(void* handle, ECursorMode mode) = 0;

        /**
         * \brief Whether the window should close or not
         * \param handle The target window's handle
         * \return True if the window should close. False otherwise
         */
        virtual bool shouldClose(void* handle) const = 0;

        /**
         * \brief Sets whether the window should close or not
         * \param handle The target window's handle
         * \param shouldClose Whether the window should close or not
         */
        virtual void setShouldClose(void* handle, bool shouldClose) = 0;
    };
}
