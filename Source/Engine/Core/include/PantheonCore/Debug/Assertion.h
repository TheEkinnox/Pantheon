#pragma once
#include <cstdlib>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Debug/DebugBreak.h"
#include "PantheonCore/Utility/CoreDefines.h"

#if USING(PTH_FEATURE_ASSERTION)

#define PTH_ASSERT(condition, ...) if (!(condition))      \
{                                                     \
    DEBUG_LOG_ERROR("Assertion failed: " #condition   \
    __VA_OPT__( "\n%s", PantheonCore::Utility::formatString(__VA_ARGS__).c_str()) \
    );                                                  \
    DEBUG_BREAK();                                      \
    IF_DEBUG(abort();)                                  \
} ((void)0)

#define CHECK(condition, ...) [&]() -> bool             \
{                                                       \
    if (!(condition))                                   \
    {                                                   \
        DEBUG_LOG_ERROR("Check failed: " #condition     \
        __VA_OPT__( "\n%s", PantheonCore::Utility::formatString(__VA_ARGS__).c_str()) \
        );                                              \
        DEBUG_BREAK();                                  \
        return false;                                   \
    }                                                   \
    return true;                                        \
}()

#define ASSUME(condition, ...) CHECK(condition __VA_OPT__(,) __VA_ARGS__)
#define ASSUME_FALSE(condition, ...) !CHECK(!(condition) __VA_OPT__(,) __VA_ARGS__)

#else

#define PTH_ASSERT(condition, ...) ((void)0)

#define CHECK(condition, ...) [](const bool b) -> bool \
{                                                      \
    return b;                                          \
} (condition)

#define ASSUME(condition, ...) true
#define ASSUME_FALSE(condition, ...) false

#endif // #if USING(PTH_FEATURE_ASSERTION)
