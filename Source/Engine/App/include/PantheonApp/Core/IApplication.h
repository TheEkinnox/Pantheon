#pragma once

#include "PantheonApp/Core/Context.h"

namespace PantheonEngine::Application::Core
{
    class IApplication
    {
    public:
        /**
         * \brief Disable the Application's copy constructor
         */
        IApplication(const IApplication&) = delete;

        /**
         * \brief Disable the Application's move constructor
         */
        IApplication(IApplication&&) = delete;

        /**
         * \brief Virtual destructor
         */
        virtual ~IApplication() = default;

        /**
         * \brief Disable the Application's copy assignment operator
         */
        IApplication& operator=(const IApplication&) = delete;

        /**
         * \brief Disable the Application's move assignment operator
         */
        IApplication& operator=(IApplication&&) = delete;

        /**
         * \brief Starts the application's main loop
         */
        void run(int argc, char* argv[]);

        /**
         * \brief Whether the application is currently running or not.
         * \return True if the application is running. False otherwise.
         */
        virtual bool isRunning() const = 0;

    protected:
        /**
         * \brief Creates an application with the given context
         */
        explicit IApplication(std::unique_ptr<Context>);

        /**
         * \brief Provides read access to the application's context
         * \return A reference to the application's context
         */
        const Context& getContext() const;

        /**
         * \brief Called when the application starts
         */
        virtual void onStart(int argc, char* argv[]) = 0;

        /**
         * \brief Called on each update cycle
         */
        virtual void onUpdate() = 0;

        /**
         * \brief Called every fixed time step
         */
        virtual void onFixedUpdate() = 0;

        /**
         * \brief Called when the application stops
         */
        virtual void onStop() = 0;

    private:
        std::unique_ptr<Context> m_context = nullptr;
    };
}
