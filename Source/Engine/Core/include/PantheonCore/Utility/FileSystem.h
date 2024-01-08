#pragma once
#include <vector>
#include <string>

namespace PantheonEngine::Core::Utility
{
    /**
     * \brief Reads the lines of the given text file
     * \param fileName The file's path
     * \return A vector containing the file's lines
     */
    std::vector<std::string> readLines(const std::string& fileName);

    /**
     * \brief Gets the calling application's directory
     * \return The calling application's directory
     */
    const char* getApplicationDirectory();

    /**
     * \brief Gets the current working directory
     * \return The current working directory
     */
    std::string getWorkingDirectory();

    /**
     * \brief Sets the application's working directory
     * \param dir The new working directory
     * \return True on success. False otherwise
     */
    bool changeDirectory(const std::string& dir);
}
