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

#ifndef PANTHEON_API
#define PANTHEON_API __declspec(dllexport)
#endif //!PANTHEON_API

#ifndef ARGS
#define ARGS(...) __VA_ARGS__
#endif //!ARGS

#ifndef EXPORT_NEW

#define EXPORT_NEW(Prefix, Suffix, Type, CtorArgs, ...)         \
PANTHEON_API Type* Prefix##Type##_New##Suffix##CtorArgs         \
{                                                               \
    return new Type(__VA_ARGS__);                               \
}

#endif //!EXPORT_NEW

#ifndef EXPORT_NEW_DEFAULT
#define EXPORT_NEW_DEFAULT(Prefix, Type) EXPORT_NEW(Prefix, _Default, Type, (), /**/)
#endif //!EXPORT_NEW_DEFAULT

#ifndef EXPORT_DELETE

#define EXPORT_DELETE(Prefix, Type)                         \
PANTHEON_API void Prefix##Type##_Delete(Type* ptr)          \
{                                                           \
    if (ptr != nullptr) delete ptr; ptr = nullptr;          \
}

#endif //!EXPORT_DELETE

#ifndef EXPORT_FUNC

#define EXPORT_FUNC(Prefix, Suffix, Type, ReturnType, FuncName, FuncArgs, Params, DefaultAction)    \
PANTHEON_API ReturnType Prefix##Type##_##FuncName##Suffix(Type* ptr, FuncArgs)                      \
{                                                                                                   \
    if (ptr != nullptr) return ptr->FuncName(Params);                                               \
                                                                                                    \
    DefaultAction;                                                                                  \
}

#endif //!EXPORT_FUNC

#ifndef EXPORT_FUNC_NO_PARAMS

#define EXPORT_FUNC_NO_PARAMS(Prefix, Suffix, Type, ReturnType, FuncName, DefaultAction)    \
PANTHEON_API ReturnType Prefix##Type##_##FuncName##Suffix(Type* ptr)                        \
{                                                                                           \
    if (ptr != nullptr) return ptr->FuncName();                                             \
                                                                                            \
    DefaultAction;                                                                          \
}

#endif //!EXPORT_FUNC_NO_PARAMS

#ifdef PTH_SERVICE

#ifndef EXPORT_SERVICE

#define EXPORT_SERVICE(Prefix, Suffix, Type)                            \
static void PANTHEON_API Prefix##Type##_##provideService##Suffix(Type* ptr)    \
{                                                                       \
    ASSERT(ptr);                                                        \
    PantheonEngine::Core::Utility::ServiceLocator::provide(*ptr);       \
}

#endif //!EXPORT_SERVICE

#ifndef EXPORT_SERVICE_FUNC

#define EXPORT_SERVICE_FUNC(Prefix, Suffix, Type, ReturnType, FuncName, FuncArgs, Params)    \
PANTHEON_API ReturnType Prefix##Type##_##FuncName##Suffix(FuncArgs)                          \
{                                                                                            \
    return PTH_SERVICE(Type).FuncName(Params);                                               \
}

#endif //!EXPORT_SERVICE_FUNC

#ifndef EXPORT_SERVICE_FUNC_NO_PARAMS

#define EXPORT_SERVICE_FUNC_NO_PARAMS(Prefix, Suffix, Type, ReturnType, FuncName)    \
PANTHEON_API ReturnType Prefix##Type##_##FuncName##Suffix()                          \
{                                                                                    \
    return PTH_SERVICE(Type).FuncName();                                             \
}

#endif //!EXPORT_SERVICE_FUNC_NO_PARAMS

#endif //PTH_SERVICE
