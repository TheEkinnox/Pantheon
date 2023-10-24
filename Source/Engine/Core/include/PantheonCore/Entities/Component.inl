#pragma once
#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Entities/Component.h"
#include "PantheonCore/Utility/macros.h"

namespace PantheonEngine::Core::Entities
{
    template <typename T>
    constexpr void Component::registerType()
    {
        static_assert(std::is_base_of_v<Component, T>);

        const std::string name = TYPE_NAME(T);

        ASSERT(s_componentTypes.contains(name), "Component type \"%s\" has already been registered", name.c_str());

        s_componentTypes[name] = [](Entity& owner) -> std::shared_ptr<Component>
        {
            return std::make_shared<T>(owner);
        };
    }

    inline std::shared_ptr<Component> Component::create(const std::string& type, Entity& owner)
    {
        return s_componentTypes.contains(type) ? s_componentTypes[type](owner) : nullptr;
    }
}
