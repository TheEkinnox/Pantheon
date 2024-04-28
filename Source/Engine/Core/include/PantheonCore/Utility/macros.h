#pragma once

#ifndef ALIGN
#define ALIGN(value, alignment) ((value) % (alignment) == 0 ? (value) : ((value) + (alignment) - (value) % (alignment)))
#endif

#ifndef ARGS
#define ARGS(...) __VA_ARGS__
#endif //!ARGS
