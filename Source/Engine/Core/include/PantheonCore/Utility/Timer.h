#pragma once
#include <chrono>

namespace PantheonEngine::Core::Utility
{
    class Timer
    {
    public:
        /**
         * \brief Creates a default timer
         */
        Timer() = default;

        /**
         * \brief Creates a timer with the given time scale
         * \param timeScale The timer's initial time scale
         */
        explicit Timer(float timeScale);

        /**
         * \brief Destroys the timer
         */
        ~Timer() = default;

        /**
         * \brief Updates the timer's info
         */
        void update();

        /**
         * \brief Gets the scaled time since the creation of this timer
         * \return The scaled time elapsed since the creation of this timer
         */
        float getTime() const;

        /**
         * \brief Gets the scaled time between the previous and last updates
         * \return The scaled time between the previous and last updates
         */
        float getDeltaTime() const;

        /**
         * \brief Gets the unscaled time since the creation of this timer
         * \return The unscaled time elapsed since the creation of this timer
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
        float m_unscaledTime = 0;
        float m_deltaTime = 0;
        float m_timeScale = 1;

        bool m_isFirstUpdate = true;
    };
}
