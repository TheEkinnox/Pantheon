#include "PantheonCore/Utility/Timer.h"

#include <chrono>

#include "Arithmetic.h"

#include "PantheonCore/Utility/ServiceLocator.h"
#include "PantheonCore/Utility/macros.h"

namespace PantheonCore::Utility
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
        EXPORT_SERVICE_FUNC(/**/, /**/, Timer, void, setTimeScale, const float timeScale, timeScale)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getFixedTime)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getFixedDeltaTime)
        EXPORT_SERVICE_FUNC(/**/, /**/, Timer, void, setFixedDeltaTime, const float deltaTime, deltaTime)
        EXPORT_SERVICE_FUNC_NO_PARAMS(/**/, /**/, Timer, float, getMaxDeltaTime)
        EXPORT_SERVICE_FUNC(/**/, /**/, Timer, void, setMaxDeltaTime, const float deltaTime, deltaTime)
    }

#pragma endregion

    Timer::Timer(const float timeScale)
        : m_timeScale(timeScale)
    {
    }

    void Timer::tick()
    {
        if (m_isFirstUpdate)
        {
            reset();
            m_currentTime = clock::now();
            m_lastUpdate  = m_currentTime;
            m_deltaTime   = 0;

            m_isFirstUpdate = false;
        }
        else
        {
            m_lastUpdate  = m_currentTime;
            m_currentTime = clock::now();
            m_deltaTime   = std::chrono::duration<float>(m_currentTime - m_lastUpdate).count();
            m_unscaledTime += m_deltaTime;
            m_time += getDeltaTime();
        }

        while (LibMath::abs(m_fixedTime - m_time) >= m_fixedDeltaTime)
        {
            m_fixedTime += m_fixedDeltaTime;
            m_onFixedUpdate.invoke(m_fixedDeltaTime);
        }

        m_onUpdate.invoke(m_deltaTime);

        m_frameCount++;
    }

    void Timer::reset()
    {
        m_isFirstUpdate = true;
        m_time          = 0;
        m_fixedTime     = 0;
        m_unscaledTime  = 0;
    }

    float Timer::getTime() const
    {
        return m_time;
    }

    float Timer::getDeltaTime() const
    {
        return LibMath::min(m_deltaTime, m_maxDeltaTime) * m_timeScale;
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

    float Timer::getFixedTime() const
    {
        return m_fixedTime;
    }

    float Timer::getFixedDeltaTime() const
    {
        return m_fixedDeltaTime;
    }

    void Timer::setFixedDeltaTime(const float deltaTime)
    {
        m_fixedDeltaTime = deltaTime;
    }

    float Timer::getMaxDeltaTime() const
    {
        return m_maxDeltaTime;
    }

    void Timer::setMaxDeltaTime(const float maxDeltaTime)
    {
        m_maxDeltaTime = maxDeltaTime;
    }

    uint64_t Timer::getFrameCount() const
    {
        return m_frameCount;
    }
}
