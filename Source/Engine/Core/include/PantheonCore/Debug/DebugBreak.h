#pragma once
#ifdef _DEBUG
    #if defined(_WIN32)
        #include <intrin.h>
        #define DEBUG_BREAK() (__nop(), __debugbreak())
    #elif defined(__unix__)
        #define DEBUG_BREAK() raise(SIGTRAP)
    #elif defined(__APPLE__)
        #include <TargetConditionals.h>
        #if TARGET_CPU_X86
            #define DEBUG_BREAK() __asm__("int $3")
        #else
            #define DEBUG_BREAK() __builtin_trap()
        #endif // TARGET_CPU_X86
    #else
        #define DEBUG_BREAK()
    #endif // _WIN32
#else
    #define DEBUG_BREAK()
#endif // _DEBUG
