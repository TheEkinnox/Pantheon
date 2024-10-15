#pragma once
#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Utility/utility.h"

#include <cassert>
#include <fstream>
#include <iostream>

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
    void Logger::print(const char* format, const ELogType type, Args... args)
    {
        const std::string message = Utility::formatString(format, args...);

        std::string prefix;

        switch (type)
        {
        case ELogType::LOG_INFO:
            prefix = "[INFO] ";
            break;
        case ELogType::LOG_WARNING:
            prefix = "[WARNING] ";
            break;
        case ELogType::LOG_ERROR:
            prefix = "[ERROR] ";
            break;
        case ELogType::LOG_RAW:
        default:
            break;
        }

        (type == ELogType::LOG_ERROR ? std::cerr : std::cout) << prefix << message << std::flush;

        if (m_filePath.empty())
            return;

        std::ofstream file(m_filePath, std::ios::app);
        assert(file.is_open());

        file << prefix << message << std::flush;
    }

    template <typename... Args>
    void Logger::debugLog(const char* file, const size_t line, const char* format, const ELogType type, Args... args)
    {
        std::string message = Utility::formatString(format, args...);

#ifdef PTH_USE_ASSERTION
        message = Utility::formatString("%s(%d): %s\n", file, line, message.c_str());
#else
        message += '\n';
        (void)sizeof(file);
        (void)sizeof(line);
#endif // PTH_USE_ASSERTION

        print(message.c_str(), type);
    }
}
