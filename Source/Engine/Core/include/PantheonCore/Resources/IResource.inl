#pragma once

#include <type_traits>

namespace PantheonEngine::Core::Resources
{
    inline bool IResource::load(const std::string& fileName)
    {
        return load(fileName.c_str());
    }

    template <typename T>
    constexpr void IResource::registerType()
    {
        static_assert(std::is_base_of_v<IResource, T>);

        const std::string name = typeid(T).raw_name();

        if (m_resourceTypes.contains(name))
            throw std::runtime_error("Resource type \"" + name + "\" has already been registered");

        m_resourceTypes[name] = []() -> IResource* {
            return new T();
        };
    }

    inline IResource* IResource::create(const std::string& type)
    {
        return m_resourceTypes.contains(type) ? m_resourceTypes[type]() : nullptr;
    }
}
