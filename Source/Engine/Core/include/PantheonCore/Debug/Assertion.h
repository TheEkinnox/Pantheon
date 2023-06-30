#pragma once

#ifndef ASSERT
#ifdef _DEBUG

#define ASSERT(condition) if (!(condition)) __debugbreak()

#else

#include <cstdlib>
#define ASSERT(condition) if (!(condition)) abort()

#endif // _DEBUG
#endif // !ASSERT
