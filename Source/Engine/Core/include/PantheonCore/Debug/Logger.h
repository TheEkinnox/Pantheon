#pragma once

#include <filesystem>
#include <fstream>

#ifndef DEBUG_LOG
#include "PantheonCore/Utility/ServiceLocator.h"
#define DEBUG_LOG(format, ...) PTH_SERVICE(PantheonEngine::Core::Debug::Logger).debugLog(__FILE__, __LINE__, format, ##__VA_ARGS__)
#endif //DEBUG_LOG

namespace PantheonEngine::Core::Debug
{
    class Logger
    {
    public:
        Logger() = default;
        Logger(const Logger& other) = delete;
        Logger(Logger&& other) noexcept;
        ~Logger();

        Logger& operator=(const Logger& other) = delete;
        Logger& operator=(Logger&& other) noexcept;

        /**
         * \brief Sets the given file as the log output.
         * \param filePath The log file's path
         */
        inline void openFile(const std::filesystem::path& filePath);

        /**
         * \brief Closes the current log output file.
         */
        inline void closeFile();

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

    private:
        std::ofstream m_file;
    };
}

#include "Logger.inl"
