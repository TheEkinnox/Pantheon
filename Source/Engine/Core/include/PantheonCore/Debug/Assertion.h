#pragma once
#include <cstdlib>

#include "PantheonCore/Debug/Logger.h"

#ifndef ASSERT
#ifdef _DEBUG

#define ASSERT(condition, ...) if (!(condition))    \
{                                                   \
    __VA_OPT__(DEBUG_LOG_ERROR(__VA_ARGS__);)       \
    __debugbreak();                                 \
    abort();                                        \
} ((void)0)

#else

#define ASSERT(condition, ...) if (!(condition))    \
{                                                   \
    __VA_OPT__(DEBUG_LOG_ERROR(__VA_ARGS__);)       \
    abort();                                        \
} ((void)0)

#endif // _DEBUG
#endif // !ASSERT
