#pragma once
#include <memory>

#include "PantheonCore/Utility/Timer.h"

namespace PantheonApp::Core
{
    namespace Exceptions
    {
        class ContextInitFailed : public std::runtime_error
        {
        public:
            ContextInitFailed()
                : runtime_error("Context initialization failed")
            {
            }

            explicit ContextInitFailed(const std::string& message)
                : runtime_error(message.c_str())
            {
            }

            explicit ContextInitFailed(const char* message)
                : runtime_error(message)
            {
            }
        };
    }

    class Context
    {
        static constexpr unsigned int PANTHEON_OPENGL_VERSION_MAJOR = 4;
        static constexpr unsigned int PANTHEON_OPENGL_VERSION_MINOR = 6;

    public:
        std::unique_ptr<PantheonCore::Utility::Timer> m_timer;

        /**
         * \brief Creates and initializes the application context
         */
        Context(bool useVsync, int sampleCount);

        /**
         * \brief Disable copy constructor
         */
        Context(const Context& other) = delete;

        /**
         * \brief Creates a move copy of the given application context
         * \param other The application context to move
         */
        Context(Context&& other) = default;

        /**
         * \brief Clears the context's previously allocated resources
         */
        virtual ~Context();

        /**
         * \brief Disable copy assignment operator
         */
        Context& operator=(const Context& other) = delete;

        /**
         * \brief Creates a move copy of the given application context
         * \param other The application context to move
         */
        Context& operator=(Context&& other) = default;

        /**
         * \brief The function to call to update the context's data
         */
        virtual void update();

        /**
         * \brief Checks whether the current application context has been correctly initialized
         * \return True if the context is initialized, false otherwise.
         */
        bool isInitialized() const;

        /**
         * \brief Checks whether vsync is currently enabled or not
         * \return True if vsync is enabled, false otherwise.
         */
        bool getVsync() const;

        /**
         * \brief Enables or disables vertical synchronisation
         * \param useVsync Whether vsync should be enabled or not
         */
        void setVsync(bool useVsync);

    protected:
        static void bindDebugCallback();

    private:
        bool m_isInitialized;
        bool m_useVsync;
    };
}
