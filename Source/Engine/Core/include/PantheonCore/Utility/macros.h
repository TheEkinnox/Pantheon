#pragma once

#ifndef ALIGN
#define ALIGN(value, alignment) ((value) % (alignment) == 0 ? (value) : ((value) + (alignment) - (value) % (alignment)))
#endif

#ifndef ARGS
#define ARGS(...) __VA_ARGS__
#endif //!ARGS

#ifndef CAT
#undef CAT_
#define CAT_(a, b) a##b
#define CAT(a, b) CAT_(a, b)
#endif

#ifndef STRINGIFY
#undef STRINGIFY_
#define STRINGIFY_(x) #x
#define STRINGIFY(x) STRINGIFY_(x)
#endif

#define UNIQUE_VAR(x) CAT(_, CAT(x, __LINE__))