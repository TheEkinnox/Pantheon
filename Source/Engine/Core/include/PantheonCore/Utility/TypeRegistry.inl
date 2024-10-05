#pragma once
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Utility/TypeRegistry.h"

#include <ranges>
#include <type_traits>

namespace PantheonCore::Utility
{
    template <class TypeInfo>
    template <typename T>
    TypeId TypeRegistry<TypeInfo>::getTypeId()
    {
        return typeid(std::remove_cvref_t<T>).hash_code();
    }

    template <class TypeInfo>
    template <typename T>
    TypeInfo& TypeRegistry<TypeInfo>::registerType(const std::string& name, const TypeInfo& info)
    {
        PTH_ASSERT(!m_typeIds.contains(name), "Type name \"%s\" has already been registered", name.c_str());

        TypeId id = getTypeId<T>();
        PTH_ASSERT(!m_typeInfos.contains(id), "Type %llu (\"%s\") has already been registered", id, typeid(T).name());

        m_typeNames[id] = name;
        m_typeIds[name] = id;
        return (m_typeInfos[id] = info);
    }

    template <class TypeInfo>
    std::vector<std::string> TypeRegistry<TypeInfo>::getRegisteredNames() const
    {
        const auto view = m_typeNames | std::ranges::views::values;
        return { view.begin(), view.end() };
    }

    template <class TypeInfo>
    bool TypeRegistry<TypeInfo>::contains(const std::string& name) const
    {
        return m_typeIds.contains(name);
    }

    template <class TypeInfo>
    bool TypeRegistry<TypeInfo>::contains(const TypeId& id) const
    {
        return m_typeInfos.contains(id);
    }

    template <class TypeInfo>
    template <typename T>
    bool TypeRegistry<TypeInfo>::contains() const
    {
        return contains(getTypeId<T>());
    }

    template <class TypeInfoT>
    TypeInfoT& TypeRegistry<TypeInfoT>::getTypeInfo(const std::string& type)
    {
        const auto it = m_typeIds.find(type);
        PTH_ASSERT(it != m_typeIds.end(), "No registered type \"%s\" found.", type.c_str());
        return getTypeInfo(it->second);
    }

    template <typename TypeInfo>
    const TypeInfo& TypeRegistry<TypeInfo>::getTypeInfo(const std::string& type) const
    {
        return const_cast<TypeRegistry*>(this)->getTypeInfo(type);
    }

    template <class TypeInfoT>
    TypeInfoT& TypeRegistry<TypeInfoT>::getTypeInfo(const size_t typeId)
    {
        const auto it = m_typeInfos.find(typeId);
        PTH_ASSERT(it != m_typeInfos.end(), "No registered type id \"%llu\" found.", typeId);
        return it->second;
    }

    template <typename TypeInfo>
    const TypeInfo& TypeRegistry<TypeInfo>::getTypeInfo(const size_t typeId) const
    {
        return const_cast<TypeRegistry*>(this)->getTypeInfo(typeId);
    }

    template <class TypeInfoT>
    template <typename T>
    TypeInfoT& TypeRegistry<TypeInfoT>::getTypeInfo()
    {
        return getTypeInfo(getTypeId<T>());
    }

    template <typename TypeInfo>
    template <typename T>
    const TypeInfo& TypeRegistry<TypeInfo>::getTypeInfo() const
    {
        return getTypeInfo(getTypeId<T>());
    }

    template <typename TypeInfo>
    const std::string& TypeRegistry<TypeInfo>::getRegisteredTypeName(const size_t typeId) const
    {
        const auto it = m_typeNames.find(typeId);
        PTH_ASSERT(it != m_typeNames.end(), "Couldn't find registered name of type with id %llu", typeId);

        return it->second;
    }

    template <typename TypeInfo>
    template <typename T>
    const std::string& TypeRegistry<TypeInfo>::getRegisteredTypeName() const
    {
        return getRegisteredTypeName(getTypeId<T>());
    }
}
