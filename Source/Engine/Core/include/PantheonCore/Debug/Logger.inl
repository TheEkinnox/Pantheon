#pragma once
#include <cassert>
#include <fstream>
#include <iostream>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/utility.h"

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN	// Disables unnecessary windows features
#define NOMINMAX			// Avoids conflicts with LibMath's min and max
#undef APIENTRY				// Avoids conflicts with some other libs (e.g: GLAD)
#include <Windows.h>
#endif

namespace PantheonEngine::Core::Debug
{
    inline void Logger::setFile(const std::filesystem::path& filePath)
    {
        m_filePath = filePath;
    }

    inline Logger& Logger::getInstance()
    {
        static Logger instance;
        return instance;
    }

    template <typename... Args>
    void Logger::print(const char* format, const bool isError, Args... args)
    {
        const std::string message = Utility::formatString(format, args...);

        (isError ? std::cerr : std::cout) << message << std::flush;

        if (m_filePath.empty())
            return;

        std::ofstream file(m_filePath);
        assert(file.is_open());
        file << message << std::flush;
    }

    template <typename... Args>
    void Logger::debugLog(const char* file, const size_t line, const char* format, const bool isError, Args... args)
    {
        std::string message = Utility::formatString(format, args...);
        message = Utility::formatString("%s(%d): %s", file, line, message.c_str());

        print(message.c_str(), isError);

#ifdef _WINDOWS_
        OutputDebugStringA(message.c_str());
#endif
    }
}
