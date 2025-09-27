#pragma once
#include <cstring>

namespace PantheonCore::Utility
{
#if __STDC_WANT_SECURE_LIB__

    inline bool memCopy(void* destination, const size_t destinationSize, const void* source, const size_t sourceSize)
    {
        return memcpy_s(destination, destinationSize, source, sourceSize) == 0;
    }

    inline bool strCopy(char* destination, const size_t destinationSize, const char* source)
    {
        return strcpy_s(destination, destinationSize, source) == 0;
    }

#else // __STDC_WANT_SECURE_LIB__

    inline bool memCopy(void* destination, const size_t destinationSize, const void* source, size_t sourceSize)
    {
        return destinationSize >= sourceSize && memcpy(destination, source, sourceSize) == destination;
    }

    inline bool strCopy(char* destination, const size_t destinationSize, const char* source)
    {
        return destinationSize >= strlen(source) && strcpy(destination, source) == destination;
    }

#endif // __STDC_WANT_SECURE_LIB__
}
