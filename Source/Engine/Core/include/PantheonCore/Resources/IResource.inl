#pragma once

#include <type_traits>

#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Resources/IResource.h"

namespace PantheonEngine::Core::Resources
{
    template <typename T>
    constexpr void IResource::registerType(const std::string& name)
    {
        static_assert(std::is_base_of_v<IResource, T>);

        ASSERT(!s_resourceTypes.contains(name), "Resource type \"%s\" has already been registered", name.c_str());

        s_resourceTypes[name] = []() -> IResource* {
            return new T();
        };
    }

    template <typename T>
    std::string IResource::getRegisteredTypeName()
    {
        static_assert(std::is_base_of_v<IResource, T>);

        const auto it = s_resourceTypeNames.find(typeid(T).hash_code());
        ASSERT(it != s_resourceTypeNames.end());

        return it->second;
    }

    inline IResource* IResource::create(const std::string& type)
    {
        return s_resourceTypes.contains(type) ? s_resourceTypes[type]() : nullptr;
    }
}
