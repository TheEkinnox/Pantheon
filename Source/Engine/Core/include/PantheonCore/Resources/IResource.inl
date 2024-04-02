#pragma once
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Resources/IResource.h"

#include <type_traits>

namespace PantheonCore::Resources
{
    template <typename T, typename... Args>
    T* createResource(Args&&... args)
    {
        return new T(std::forward<Args>(args)...);
    }

    template <typename T>
    T* getDefaultResource()
    {
        return nullptr;
    }
}
