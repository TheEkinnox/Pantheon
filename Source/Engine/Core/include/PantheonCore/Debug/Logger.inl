#pragma once
#include <cassert>
#include <fstream>
#include <iostream>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/utility.h"

namespace PantheonCore::Debug
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

#if defined(_DEBUG) || defined(PTH_VERBOSE_LOG)
        message = Utility::formatString("%s(%d): %s\n", file, line, message.c_str());
#else
        message += '\n';
        (void)sizeof(file);
        (void)sizeof(line);
#endif // _DEBUG || PTH_VERBOSE_LOG

        print(message.c_str(), isError);
    }
}
