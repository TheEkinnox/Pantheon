#pragma once

namespace PantheonEngine::Application::Windowing
{
    struct WindowSettings
    {
        static constexpr int DONT_CARE = -1;

        const char* m_title;

        int m_width;
        int m_height;
        int m_minWidth = DONT_CARE;
        int m_minHeight = DONT_CARE;
        int m_maxWidth = DONT_CARE;
        int m_maxHeight = DONT_CARE;
    };
}
