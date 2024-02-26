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

    template <typename T>
    constexpr void IResource::registerType(const std::string& name)
    {
        static_assert(std::is_base_of_v<IResource, T>);

        ASSERT(!s_resourceTypes.contains(name), "Resource type \"%s\" has already been registered", name.c_str());

        const size_t typeHash = typeid(T).hash_code();
        ASSERT(!s_resourceTypeNames.contains(typeHash), "Resource type \"%s\" has already been registered", name.c_str());

        const auto allocFunc = []
        {
            return static_cast<IResource*>(createResource<T>());
        };

        const auto getDefaultFunc = []
        {
            return static_cast<IResource*>(getDefaultResource<T>());
        };

        s_resourceTypes[name] =
        {
            allocFunc,
            getDefaultFunc
        };

        s_resourceTypeNames[typeHash] = name;
    }

    template <typename T>
    const std::string& IResource::getRegisteredTypeName()
    {
        static_assert(std::is_base_of_v<IResource, T>);

        const auto it = s_resourceTypeNames.find(typeid(T).hash_code());
        ASSERT(it != s_resourceTypeNames.end());

        return it->second;
    }

    inline IResource* IResource::create(const std::string& type)
    {
        const auto it = s_resourceTypes.find(type);
        return it != s_resourceTypes.end() ? it->second.allocate() : nullptr;
    }

    inline IResource* IResource::getDefault(const std::string& type)
    {
        const auto it = s_resourceTypes.find(type);
        return it != s_resourceTypes.end() ? it->second.getDefault() : nullptr;
    }
}
