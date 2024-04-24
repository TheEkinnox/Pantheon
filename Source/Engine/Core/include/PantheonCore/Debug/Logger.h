#pragma once

#include "PantheonCore/Debug/ELogType.h"

#include <filesystem>

#ifndef DEBUG_LOG
#define DEBUG_LOG(format, ...) PantheonCore::Debug::Logger::getInstance().debugLog(__FILE__, __LINE__, format, PantheonCore::Debug::ELogType::LOG_INFO __VA_OPT__(,) __VA_ARGS__)
#define DEBUG_LOG_WARNING(format, ...) PantheonCore::Debug::Logger::getInstance().debugLog(__FILE__, __LINE__, format, PantheonCore::Debug::ELogType::LOG_WARNING __VA_OPT__(,) __VA_ARGS__)
#define DEBUG_LOG_ERROR(format, ...) PantheonCore::Debug::Logger::getInstance().debugLog(__FILE__, __LINE__, format, PantheonCore::Debug::ELogType::LOG_ERROR __VA_OPT__(,) __VA_ARGS__)
#endif //DEBUG_LOG

namespace PantheonCore::Debug
{
    class Logger
    {
    public:
        Logger()                        = default;
        Logger(const Logger& other)     = default;
        Logger(Logger&& other) noexcept = default;
        ~Logger()                       = default;

        Logger& operator=(const Logger& other)     = default;
        Logger& operator=(Logger&& other) noexcept = default;

        /**
         * \brief Sets the given file as the log output.
         * \param filePath The log file's path
         */
        inline void setFile(const std::filesystem::path& filePath);

        /**
         * \brief Logs a message with the given format following printf's syntax.
         * \tparam Args The arguments to insert into the format string
         * \param format The format of the message
         * \param type The type of message getting logged
         * \param args Additional arguments to insert into the message
         */
        template <typename... Args>
        void print(const char* format, ELogType type, Args... args);

        /**
         * \brief Logs a message with the given format following printf's syntax.
         * Appends the given file path and line at the beginning of the message
         * \tparam Args The arguments to insert into the format string
         * \param file The file for which the function was called
         * \param line The line for which the function was called
         * \param format The format of the message
         * \param type The type of message getting logged
         * \param args Additional arguments to insert into the message
         */
        template <typename... Args>
        void debugLog(const char* file, size_t line, const char* format, ELogType type, Args... args);

        /**
        * \brief Accessor to the Logger singleton
        * \return A reference to the current Logger instance
        */
        inline static Logger& getInstance();

    private:
        std::filesystem::path m_filePath;
    };
}

#include "PantheonCore/Debug/Logger.inl"
