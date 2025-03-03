#pragma once
#include "PantheonCore/Utility/CoreDefines.h"

#if USING(PTH_PLATFORM_WINDOWS)

// Disable less common MFC features
#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN
#endif

// Disable unnecessary windows features
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

// Disable windows defined min and max macros
#ifndef NOMINMAX
#define NOMINMAX
#endif

// Avoids conflicts with some other libs (e.g: GLFW)
#undef APIENTRY

#if USING(PTH_COMPILER_MSVC)
__pragma( warning( push ) )
__pragma( warning( disable : 4201 ) ) // nonstandard extension used: nameless struct/union
#endif // _MSC_VER

#include <Windows.h>

#if USING(PTH_COMPILER_MSVC)
__pragma( warning( pop ) )
#endif // _MSC_VER

#endif // #if USING(PTH_PLATFORM_WINDOWS)
