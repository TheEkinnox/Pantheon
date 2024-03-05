#include "PantheonCore/Utility/FileSystem.h"

#include "PantheonCore/Debug/Logger.h"

// Platform specific defines to handle getApplicationDirectory()
#if defined(_WIN32)
#if __cplusplus
extern "C"
{
#endif
    __declspec(dllimport) unsigned long __stdcall GetModuleFileNameA(void* hModule, void* lpFilename, unsigned long nSize);
    __declspec(dllimport) unsigned long __stdcall GetModuleFileNameW(void* hModule, void* lpFilename, unsigned long nSize);
    __declspec(dllimport) int __stdcall WideCharToMultiByte(unsigned int cp, unsigned long flags, void* widestr, int cchwide,
                                                            void* str, int cbmb, void* defchar, int* used_default);
#ifdef __cplusplus
}
#endif

#elif defined(__linux__)
#include <unistd.h>
#elif defined(__APPLE__)
#include <sys/syslimits.h>
#include <mach-o/dyld.h>
#endif // OSs

#if defined(_WIN32)
#include <direct.h>     // Required for: _chdir()
#define CHDIR _chdir
#define MAX_PATH 260
#else
#include <unistd.h>     // Required for: chdir()
#define CHDIR chdir
#endif

#ifndef MAX_PATH_LENGTH
#define MAX_PATH_LENGTH 4096
#endif

#ifndef PATH_SEPARATOR
#if defined(_WIN32)
#define PATH_SEPARATOR '\\'
#else
#define PATH_SEPARATOR '/'
#endif
#endif

namespace PantheonCore::Utility
{
    std::vector<std::string> readLines(const std::string& fileName)
    {
        std::ifstream fs(fileName);

        if (!fs.is_open())
        {
            DEBUG_LOG_ERROR("Unable to open file at path \"%s\"", fileName.c_str());
            return {};
        }

        std::vector<std::string> lines;
        std::string              line;

        while (std::getline(fs, line))
            lines.emplace_back(std::move(line));

        return lines;
    }

    const char* getApplicationDirectory()
    {
        static char appDir[MAX_PATH_LENGTH] = { 0 };
        memset(appDir, 0, MAX_PATH_LENGTH);

#if defined(_WIN32)
        using length_t = int;
        length_t len = 0;

#if defined(UNICODE)
        unsigned short widePath[MAX_PATH];
        len = static_cast<length_t>(GetModuleFileNameW(nullptr, widePath, MAX_PATH));
        len = WideCharToMultiByte(0, 0, widePath, len, appDir, MAX_PATH, nullptr, nullptr);
#else
        len = static_cast<length_t>(GetModuleFileNameA(nullptr, appDir, MAX_PATH));
#endif

#elif defined(__linux__)
        using length_t = ssize_t;
        unsigned int size = MAX_PATH_LENGTH;
        length_t len = readlink("/proc/self/exe", appDir, size);
#elif defined(__APPLE__)
        using length_t = int;
        uint32_t size = MAX_PATH_LENGTH;
        length_t len = 0;

        if (_NSGetExecutablePath(appDir, &size) == 0)
            len = strlen(appDir);
#endif

        const auto tmp = std::filesystem::canonical(len > 0 ? appDir : "./").remove_filename();
        strcpy_s(appDir, MAX_PATH_LENGTH, tmp.string().c_str());

        return appDir;
    }

    std::string getWorkingDirectory()
    {
        auto tmp = std::filesystem::current_path().string();

        if (!tmp.ends_with(PATH_SEPARATOR))
            tmp += PATH_SEPARATOR;

        return tmp;
    }

    bool changeDirectory(const std::string& dir)
    {
        return CHDIR(dir.c_str()) == 0;
    }

    std::string appendPath(const std::string& root, const std::string& path)
    {
        return std::filesystem::path(root).append(path).string();
    }

    bool pathExists(const std::string& path)
    {
        return std::filesystem::exists(path);
    }
}
