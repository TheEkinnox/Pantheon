#pragma once
#include <cstdlib>

#include "PantheonCore/Debug/Logger.h"
#include "PantheonCore/Debug/DebugBreak.h"

#ifndef ASSERT
#if defined(_DEBUG) || defined(PTH_VERBOSE_LOG)

#define ASSERT(condition, ...) if (!(condition))      \
{                                                     \
    DEBUG_LOG_ERROR("Assertion failed: " #condition); \
    __VA_OPT__(DEBUG_LOG_ERROR(__VA_ARGS__);)         \
    DEBUG_BREAK();                                    \
    abort();                                          \
} ((void)0)

#else

#define ASSERT(condition, ...) ((void)0)

#endif // _DEBUG || PTH_VERBOSE_LOG
#endif // !ASSERT

#ifndef CHECK
#if defined(_DEBUG) || defined(PTH_VERBOSE_LOG)

#define CHECK(condition, ...) [&]() -> bool             \
{                                                       \
    if (!(condition))                                   \
    {                                                   \
        DEBUG_LOG_ERROR("Check failed: " #condition);   \
        __VA_OPT__(DEBUG_LOG_ERROR(__VA_ARGS__);)       \
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

#endif // _DEBUG || PTH_VERBOSE_LOG
#endif // !CHECK

#ifndef ASSUME
#if defined(_DEBUG) || defined(PTH_VERBOSE_LOG)

#define ASSUME(condition, ...) CHECK(condition __VA_OPT__(,) __VA_ARGS__)

#else

#define ASSUME(condition, ...) true

#endif // _DEBUG || PTH_VERBOSE_LOG
#endif // !ASSUME
