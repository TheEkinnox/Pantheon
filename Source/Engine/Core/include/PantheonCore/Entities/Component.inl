#pragma once
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Entities/Component.h"

namespace PantheonEngine::Core::Entities
{
    template <typename T>
    constexpr void Component::registerType(const std::string& name)
    {
        static_assert(std::is_base_of_v<Component, T>);

        ASSERT(!s_componentTypes.contains(name), "Component type \"%s\" has already been registered", name.c_str());

        s_componentTypes[name] = [](Entity& owner) -> std::shared_ptr<Component>
        {
            return std::make_shared<T>(owner);
        };
    }

    template <typename T>
    std::string Component::getRegisteredTypeName()
    {
        static_assert(std::is_base_of_v<Component, T>);

        const auto it = s_componentTypeNames.find(typeid(T).hash_code());
        ASSERT(it != s_componentTypeNames.end());

        return it->second;
    }

    inline std::shared_ptr<Component> Component::create(const std::string& type, Entity& owner)
    {
        return s_componentTypes.contains(type) ? s_componentTypes[type](owner) : nullptr;
    }
}
