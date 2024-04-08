#pragma once
#include "PantheonScripting/LuaContext.h"

namespace PantheonScripting
{
    template <typename... Args>
    bool LuaContext::tryCall(LuaScriptComponent& component, const std::string& name, Args&&... args)
    {
        if (!m_isValid || !component.m_table.valid())
            return false;

        sol::protected_function func = component.m_table[name];

        if (!func.valid())
            return false;

        const auto result = func.call(component.m_table, std::forward<Args>(args)...);

        if (!result.valid())
        {
            [[maybe_unused]] const sol::error err = result;
            return CHECK(false, "Call to function %s of script %s failed - %s",
                    name.c_str(), component.m_script.getPath().c_str(), err.what());
        }

        return true;
    }
}
