#include "PantheonCore/Utility/Timer.h"

#include <chrono>

#include "PantheonCore/Utility/ServiceLocator.h"
#include "PantheonCore/Utility/macros.h"

namespace PantheonEngine::Core::Utility
{
#pragma region BINDINGS

    extern "C"
    {
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getTime)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getDeltaTime)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getUnscaledTime)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getUnscaledDeltaTime)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, uint64_t, getFrameCount)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getTimeScale)
        EXPORT_SERVICE_FUNC(/**/, /**/, Timer, void, setTimeScale, const float timeScale, (timeScale))
    }

#pragma endregion

    Timer::Timer(const float timeScale)
        : m_timeScale(timeScale)
    {
    }

    void Timer::update()
    {
        if (m_isFirstUpdate)
        {
            m_currentTime = clock::now();
            m_lastUpdate = m_currentTime;
            m_deltaTime = 0;

            m_isFirstUpdate = false;
        }
        else
        {
            m_lastUpdate = m_currentTime;
            m_currentTime = clock::now();
            m_deltaTime = std::chrono::duration<float>(m_currentTime - m_lastUpdate).count();
            m_unscaledTime += m_deltaTime;
            m_time += m_deltaTime * m_timeScale;
        }

        m_frameCount++;
    }

    float Timer::getTime() const
    {
        return m_time;
    }

    float Timer::getDeltaTime() const
    {
        return m_deltaTime * m_timeScale;
    }

    float Timer::getUnscaledTime() const
    {
        return m_unscaledTime;
    }

    float Timer::getUnscaledDeltaTime() const
    {
        return m_deltaTime;
    }

    float Timer::getTimeScale() const
    {
        return m_timeScale;
    }

    void Timer::setTimeScale(const float timeScale)
    {
        m_timeScale = timeScale;
    }

    uint64_t Timer::getFrameCount() const
    {
        return m_frameCount;
    }
}
