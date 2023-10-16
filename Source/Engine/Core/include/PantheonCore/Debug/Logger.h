#pragma once

#include <filesystem>

#ifndef DEBUG_LOG
#define DEBUG_LOG(format, ...) PantheonEngine::Core::Debug::Logger::getInstance().debugLog(__FILE__, __LINE__, format, false, ##__VA_ARGS__)
#define DEBUG_LOG_ERROR(format, ...) PantheonEngine::Core::Debug::Logger::getInstance().debugLog(__FILE__, __LINE__, format, true, ##__VA_ARGS__)
#endif //DEBUG_LOG

namespace PantheonEngine::Core::Debug
{
    class Logger
    {
    public:
        Logger() = default;
        Logger(const Logger& other) = default;
        Logger(Logger&& other) noexcept = default;
        ~Logger() = default;

        Logger& operator=(const Logger& other) = default;
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
         * \param isError Whether the message is an error message or not
         * \param args Additional arguments to insert into the message
         */
        template <typename... Args>
        void print(const char* format, bool isError = false, Args... args);

        /**
         * \brief Logs a message with the given format following printf's syntax.
         * Appends the given file path and line at the beginning of the message
         * \tparam Args The arguments to insert into the format string
         * \param file The file for which the function was called
         * \param line The line for which the function was called
         * \param format The format of the message
         * \param isError Whether the message is an error message or not
         * \param args Additional arguments to insert into the message
         */
        template <typename... Args>
        void debugLog(const char* file, size_t line, const char* format, bool isError, Args... args);

        /**
        * \brief Accessor to the Logger singleton
        * \return A reference to the current Logger instance
        */
        inline static Logger& getInstance();

    private:
        std::filesystem::path m_filePath;
    };
}

#include "Logger.inl"
