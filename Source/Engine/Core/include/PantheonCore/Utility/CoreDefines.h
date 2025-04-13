#pragma once
#include "PantheonCore/Utility/Using.h"

// =============
// = PLATFORMS =
// =============
#if defined(__APPLE__)
    #define PTH_PLATFORM_APPLE IN_USE
    #include <TargetConditionals.h>
#else
#define PTH_PLATFORM_APPLE NOT_IN_USE
#endif

#if defined(_WIN32) || defined(WIN32) || defined(__MINGW32__) || defined(NT)
#define PTH_PLATFORM_WINDOWS IN_USE
#else
#define PTH_PLATFORM_WINDOWS NOT_IN_USE
#endif

#define PTH_PLATFORM_IOS USE_IF(USING(PTH_PLATFORM_APPLE) && TARGET_OS_IPHONE)
#define PTH_PLATFORM_OSX USE_IF(USING(PTH_PLATFORM_APPLE) && TARGET_OS_OSX)

#if defined(__ANDROID__) || defined(ANDROID)
#define PTH_PLATFORM_ANDROID IN_USE
#else
#define PTH_PLATFORM_ANDROID NOT_IN_USE
#endif

#ifdef __linux__
#define PTH_PLATFORM_LINUX USE_IF(!USING(PTH_PLATFORM_APPLE) && !USING(PTH_PLATFORM_ANDROID))
#else
#define PTH_PLATFORM_LINUX NOT_IN_USE
#endif

#ifdef __EMSCRIPTEN__
#define PTH_PLATFORM_EMSCRIPTEN IN_USE
#else
#define PTH_PLATFORM_EMSCRIPTEN NOT_IN_USE
#endif

#define PTH_PLATFORM_DESKTOP USE_IF(USING(PTH_PLATFORM_WINDOWS) || USING(PTH_PLATFORM_LINUX) || USING(PTH_PLATFORM_OSX)))
#define PTH_PLATFORM_MOBILE USE_IF(USING(PTH_PLATFORM_ANDROID) || USING(PTH_PLATFORM_IOS))

// ===========
// = CONFIGS =
// ===========
#if defined(_DEBUG) || defined(DEBUG)
#define PTH_CONFIG_DEBUG IN_USE
#define IF_DEBUG(x) x
#else
#define PTH_CONFIG_DEBUG NOT_IN_USE
#define IF_DEBUG(x)
#endif

// =============
// = COMPILERS =
// =============
#if defined(__GNUC__)
#define PTH_COMPILER_GCC IN_USE
#else
#define PTH_COMPILER_GCC NOT_IN_USE
#endif

#if defined(__clang__)
#define PTH_COMPILER_CLANG IN_USE
#else
#define PTH_COMPILER_CLANG NOT_IN_USE
#endif

#if defined(_MSC_VER) && !USING(PTH_COMPILER_CLANG)
#define PTH_COMPILER_MSVC IN_USE
#else
#define PTH_COMPILER_MSVC NOT_IN_USE
#endif

// ===========
// = TARGETS =
// ===========
#ifdef PTH_EDITOR
#define PTH_TARGET_EDITOR IN_USE
#else
#define PTH_TARGET_EDITOR NOT_IN_USE
#endif

#ifdef PTH_STANDALONE
#define PTH_TARGET_STANDALONE IN_USE
#else
#define PTH_TARGET_STANDALONE NOT_IN_USE
#endif

#if defined(PTH_HEADLESS) || defined(PTH_HEADLESS_TEST)
#define PTH_TARGET_HEADLESS IN_USE
#else
#define PTH_TARGET_HEADLESS NOT_IN_USE
#endif

// ============
// = FEATURES =
// ============
#if USING(PTH_CONFIG_DEBUG) || defined(PTH_VERBOSE_LOG)
#define PTH_FEATURE_ASSERTION IN_USE
#define IF_ASSERT(x) x
#else
#define PTH_FEATURE_ASSERTION NOT_IN_USE
#define IF_ASSERT(x)
#endif

#if defined(UNICODE) || defined(_UNICODE)
#define PTH_FEATURE_UNICODE IN_USE
#else
#define PTH_FEATURE_UNICODE NOT_IN_USE
#endif

#define PTH_FEATURE_OPENGL USE_IF(!USING(PTH_TARGET_HEADLESS))
