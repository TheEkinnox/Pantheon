#pragma once
#include "PantheonCore/Utility/DynamicTypeInfo.h"

namespace PantheonCore::Utility
{
    template <class TypeInfo>
    template <typename... Args>
    DynamicTypeInfo<TypeInfo>::DynamicTypeInfo(Args&&... args)
        : TypeInfo(std::forward<Args>(args)...)
    {
    }

    template <class TypeInfo>
    bool DynamicTypeInfo<TypeInfo>::add(const std::string& name, const Callable& callable)
    {
        const auto it = m_callables.find(name);

        if (it != m_callables.end())
            return false;

        m_callables[name] = callable;
        return true;
    }

    template <class TypeInfo>
    void DynamicTypeInfo<TypeInfo>::replace(const std::string& name, const Callable& callable)
    {
        m_callables[name] = callable;
    }

    template <class TypeInfo>
    bool DynamicTypeInfo<TypeInfo>::remove(const std::string& name)
    {
        const auto it = m_callables.find(name);

        if (it != m_callables.end())
        {
            m_callables.erase(it);
            return true;
        }

        return false;
    }

    template <class TypeInfo>
    bool DynamicTypeInfo<TypeInfo>::has(const std::string& name) const
    {
        return m_callables.contains(name);
    }

    template <class TypeInfo>
    template <typename T>
    DynamicCallResult<T> DynamicTypeInfo<TypeInfo>::call(const std::string& name, void* payload) const
    {
        const auto it = m_callables.find(name);

        if constexpr (std::is_same_v<void, T>)
            return it != m_callables.end() ? ((void)it->second(payload), true) : false;
        else
            return it != m_callables.end() ? std::make_optional(std::any_cast<T>(it->second(payload))) : std::nullopt;
    }
}
