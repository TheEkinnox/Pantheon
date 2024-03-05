#pragma once
#include "PantheonCore/Utility/Timer.h"

#include "PantheonRendering/RHI/EGraphicsAPI.h"

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
         * \brief Sets the context's main window
         * \param handle The target window handle
         */
        virtual void setMainWindow(void* handle) = 0;

        /**
         * \brief Swaps the render buffers
         */
        virtual void swapBuffers() = 0;

        /**
         * \brief Updates the context's data
         */
        virtual void update();

        /**
         * \brief Creates an application context for the given graphics api with the given settings
         * \param backend The target graphics api
         * \param useVsync Whether vsync should be enabled or not
         * \param sampleCount The number of samples to use
         * \return The created application context
         */
        static std::unique_ptr<IContext> create(PantheonRendering::RHI::EGraphicsAPI backend, bool useVsync, int sampleCount);

    protected:
        IContext();
    };
}
