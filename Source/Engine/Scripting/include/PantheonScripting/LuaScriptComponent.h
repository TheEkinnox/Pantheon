#pragma once
#include "PantheonScripting/LuaScript.h"

#include <PantheonCore/ECS/ComponentTraits.h>
#include <PantheonCore/Resources/ResourceRef.h>

#include <sol/sol.hpp>

namespace PantheonScripting
{
    struct LuaScriptComponent
    {
        PantheonCore::Resources::ResourceRef<LuaScript> m_script;

        sol::table m_table = sol::nil;
    };
}

namespace PantheonCore::ECS
{
    /**
     * \brief The action to perform when a lua script component is added
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief The action to perform when a lua script component is removed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief The action to perform before a lua script component is changed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);

    /**
     * \brief The action to perform after a lua script component was changed
     * \param entity The component's owner
     * \param component The added component
     */
    template <>
    void ComponentTraits::onChange(EntityHandle& entity, PantheonScripting::LuaScriptComponent& component);
}
