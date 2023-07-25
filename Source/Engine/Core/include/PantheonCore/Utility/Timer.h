#pragma once
#include <chrono>

#include "PantheonCore/Eventing/Event.h"

namespace PantheonEngine::Core::Utility
{
    class Timer
    {
    public:
        Eventing::Event<> m_onUpdate;
        Eventing::Event<> m_onFixedUpdate;

        /**
         * \brief Creates a default timer
         */
        Timer() = default;

        /**
         * \brief Creates a copy of the given timer
         */
        Timer(const Timer&) = default;

        /**
         * \brief Creates a move copy of the given timer
         */
        Timer(Timer&&) = default;

        /**
         * \brief Creates a timer with the given time scale
         * \param timeScale The timer's initial time scale
         */
        explicit Timer(float timeScale);

        /**
         * \brief Creates a copy of the given timer
         */
        Timer& operator=(const Timer&) = default;

        /**
         * \brief Creates a move copy of the given timer
         */
        Timer& operator=(Timer&&) = default;

        /**
         * \brief Destroys the timer
         */
        ~Timer() = default;

        /**
         * \brief Updates the timer's info
         */
        void tick();

        /**
         * \brief Resets the timer
         */
        void reset();

        /**
         * \brief Gets the scaled time elapsed since the timer's first tick
         * \return The scaled time elapsed since the timer's first tick
         */
        float getTime() const;

        /**
         * \brief Gets the scaled time between the previous and last updates
         * \return The scaled time between the previous and last updates
         */
        float getDeltaTime() const;

        /**
         * \brief Gets the unscaled time elapsed since the timer's first tick
         * \return The unscaled time elapsed since the timer's first tick
         */
        float getUnscaledTime() const;

        /**
         * \brief Gets the unscaled time between the previous and last updates
         * \return The unscaled time between the previous and last updates
         */
        float getUnscaledDeltaTime() const;

        /**
         * \brief Gets the current time scale
         * \return The current time scale
         */
        float getTimeScale() const;

        /**
         * \brief Sets the timer's time scale
         */
        void setTimeScale(float timeScale);

        /**
         * \brief Gets the fixed time elapsed since the timer's first tick
         * \return The fixed time elapsed since the timer's first tick
         */
        float getFixedTime() const;

        /**
         * \brief Gets the timer's fixed time step
         * \return The timer's fixed time step
         */
        float getFixedDeltaTime() const;

        /**
         * \brief Sets the timer's fixed time step
         * \param deltaTime The timer's fixed time step
         */
        void setFixedDeltaTime(float deltaTime);

        /**
         * \brief Gets the timer's maximum delta time
         * \return The timer's maximum delta time
         */
        float getMaxDeltaTime() const;

        /**
         * \brief Sets the timer's maximum delta time
         * \param maxDeltaTime The timer's maximum delta time
         */
        void setMaxDeltaTime(float maxDeltaTime);

        /**
         * \brief Gets the number of time the "update" function has been called
         * \return The elapsed number of frames since the timer's creation
         */
        uint64_t getFrameCount() const;

    private:
        using clock = std::chrono::steady_clock;

        clock::time_point m_currentTime = clock::now();
        clock::time_point m_lastUpdate = clock::now();

        uint64_t m_frameCount = 0;

        float m_time = 0;
        float m_fixedTime = 0;
        float m_unscaledTime = 0;
        float m_deltaTime = 0;
        float m_fixedDeltaTime = .02f;
        float m_maxDeltaTime = .25f;
        float m_timeScale = 1.f;

        bool m_isFirstUpdate = true;
    };
}
