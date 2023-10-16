#pragma once
#include "Logger.h"
#include "PantheonCore/Utility/utility.h"
#include <iostream>

#ifdef _WINDOWS
#define WIN32_LEAN_AND_MEAN	// Disables unnecessary windows features
#define NOMINMAX			// Avoids conflicts with LibMath's min and max
#undef APIENTRY				// Avoids conflicts with some other libs (e.g: GLAD)
#include <Windows.h>
#endif

namespace PantheonEngine::Core::Debug
{
    inline Logger::Logger(Logger&& other) noexcept
        : m_file{ std::move(other.m_file) }
    {
    }

    inline Logger& Logger::operator=(Logger&& other) noexcept
    {
        if (this == &other)
            return *this;

        m_file = std::move(other.m_file);

        return *this;
    }

    inline Logger::~Logger()
    {
        if (m_file.is_open())
            m_file.close();
    }

    inline void Logger::openFile(const std::filesystem::path& filePath)
    {
        closeFile();

        m_file.open(filePath);
    }

    inline void Logger::closeFile()
    {
        if (m_file.is_open())
            m_file.close();
    }

    template <typename... Args>
    void Logger::print(const char* format, const bool isError, Args... args)
    {
        const std::string message = Utility::formatString(format, args...);

        (isError ? std::cerr : std::cout) << message << std::flush;

        if (m_file.is_open())
            m_file << message << std::flush;
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
