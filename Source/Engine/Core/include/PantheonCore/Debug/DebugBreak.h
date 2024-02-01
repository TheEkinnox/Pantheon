#pragma once

#ifdef _DEBUG
    #if defined(_WIN32)
        #define WIN32_LEAN_AND_MEAN	// Disables unnecessary windows features
        #define NOMINMAX			// Avoids conflicts with LibMath's min and max
        #include <windows.h>
        #include <intrin.h>

        inline bool isDebuggerPresent()
        {
            return IsDebuggerPresent();
        }

        #define DEBUG_BREAK_IMPL() (__nop(), __debugbreak())
    #elif defined(__unix__)
        #include <fstream>
        #include <string>
        #include <sstream>

        inline bool isDebuggerPresent()
        {
            // Adapted from https://linuxsecurity.com/features/anti-debugging-for-noobs-part-1
            std::ifstream file("/proc/self/status");
            if (!file.is_open())
                return false;

            std::string line;

            while (std::getline(file, line))
            {
                std::istringstream iss(line);
                std::string tag, value;
                iss >> tag >> value;

                if (tag == "TracerPid:" && value != "0")
                    return true;
            }

            return false;
        }

        #define DEBUG_BREAK_IMPL() raise(SIGTRAP)
    #elif defined(__APPLE__)
        #include <TargetConditionals.h>

        inline bool isDebuggerPresent()
        {
            // Adapted from https://developer.apple.com/library/archive/qa/qa1361/_index.html
            struct kinfo_proc info;
            info.kp_proc.p_flag = 0;

            size_t size = sizeof(info);

            int mib[] = { CTL_KERN, KERN_PROC, KERN_PROC_PID, getpid() };
            return sysctl(mib, sizeof(mib) / sizeof(*mib), &info, &size, NULL, 0) == 0 && (info.kp_proc.p_flag & P_TRACED) != 0;
        }

        #if TARGET_CPU_X86
            #define DEBUG_BREAK_IMPL() __asm__("int $3")
        #else
            #define DEBUG_BREAK_IMPL() __builtin_trap()
        #endif // TARGET_CPU_X86
    #else
        inline bool isDebuggerPresent()
        {
            return true;
        }

        #define DEBUG_BREAK_IMPL() *((int*)3) = 13
    #endif // _WIN32

    #define DEBUG_BREAK() ((void)(isDebuggerPresent() && (DEBUG_BREAK_IMPL(), 1)))
#else
    inline bool isDebuggerPresent()
    {
        return false;
    }

    #define DEBUG_BREAK()
#endif // _DEBUG
