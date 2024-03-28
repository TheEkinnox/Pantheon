#pragma once

#ifndef ALIGN
#define ALIGN(value, alignment) ((value) % (alignment) == 0 ? (value) : ((value) + (alignment) - (value) % (alignment)))
#endif

#ifndef SMALLEST_TYPE
#define SMALLEST_TYPE(bits) std::conditional_t<(bits) <= 8, int8_t, std::conditional_t<(bits) <= 16, int16_t, std::conditional_t<(bits) <= 32, int32_t, int64_t>>>
#endif //!SMALLEST_TYPE

#ifndef SMALLEST_UNSIGNED_TYPE

#define SMALLEST_UNSIGNED_TYPE(bits) std::conditional_t<(bits) <= 8, uint8_t,   \
    std::conditional_t<(bits) <= 16, uint16_t,                                  \
    std::conditional_t<(bits) <= 32, uint32_t, uint64_t>>>

#endif //!SMALLEST_UNSIGNED_TYPE

#ifndef ARGS
#define ARGS(...) __VA_ARGS__
#endif //!ARGS
