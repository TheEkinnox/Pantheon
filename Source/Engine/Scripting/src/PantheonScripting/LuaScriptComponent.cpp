#include "PantheonScripting/LuaScriptComponent.h"

#include "PantheonScripting/LuaContext.h"

namespace PantheonCore::ECS
{
    using namespace PantheonScripting;

    template <>
    void ComponentTraits::onAdd(EntityHandle& entity, LuaScriptComponent& component)
    {
        LuaContext& context = PTH_SERVICE(PantheonScripting::LuaContext);
        (void)context.addScript(entity, component);
    }

    template <>
    void ComponentTraits::onRemove(EntityHandle& entity, LuaScriptComponent&)
    {
        LuaContext& context = PTH_SERVICE(PantheonScripting::LuaContext);
        context.removeScript(entity);
    }

    template <>
    void ComponentTraits::onBeforeChange(EntityHandle& entity, LuaScriptComponent& component)
    {
        onRemove(entity, component);
    }

    template <>
    void ComponentTraits::onChange(EntityHandle& entity, LuaScriptComponent& component)
    {
        onAdd(entity, component);
    }
}
