#pragma once
#include <cstdlib>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Debug/DebugBreak.h"

#define PTH_USE_ASSERTION defined(_DEBUG) || defined(PTH_VERBOSE_LOG)

#ifndef PTH_ASSERT
#if PTH_USE_ASSERTION

#define PTH_ASSERT(condition, ...) if (!(condition))      \
{                                                     \
    DEBUG_LOG_ERROR("Assertion failed: " #condition   \
    __VA_OPT__( "\n%s", PantheonCore::Utility::formatString(__VA_ARGS__).c_str()) \
    );                                                \
    DEBUG_BREAK();                                    \
    abort();                                          \
} ((void)0)

#else

#define PTH_ASSERT(condition, ...) ((void)0)

#endif // PTH_USE_ASSERTION
#endif // !PTH_ASSERT

#ifndef CHECK
#if PTH_USE_ASSERTION

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

#else

#define CHECK(condition, ...) [](const bool b) -> bool \
{                                                      \
    return b;                                          \
}(condition)

#endif // PTH_USE_ASSERTION
#endif // !CHECK

#ifndef ASSUME
#if PTH_USE_ASSERTION

#define ASSUME(condition, ...) CHECK(condition __VA_OPT__(,) __VA_ARGS__)

#else

#define ASSUME(condition, ...) true

#endif // PTH_USE_ASSERTION
#endif // !ASSUME

#ifndef ASSUME_FALSE
#if PTH_USE_ASSERTION

#define ASSUME_FALSE(condition, ...) !CHECK(!(condition) __VA_OPT__(,) __VA_ARGS__)

#else

#define ASSUME_FALSE(condition, ...) false

#endif // PTH_USE_ASSERTION
#endif // !ASSUME_FALSE
