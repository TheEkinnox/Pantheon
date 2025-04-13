#pragma once
#include "PantheonScripting/LuaContext.h"

namespace PantheonScripting
{
    template <typename... Args>
    ELuaCallResult LuaContext::tryCall(sol::table& table, const std::string& name, Args&&... args)
    {
        if (!m_isValid || !table.valid())
            return ELuaCallResult::INVALID_STATE;

        sol::protected_function func = table[name];

        if (!func.valid())
            return ELuaCallResult::NOT_FOUND;

        const auto result = func.call(table, std::forward<Args>(args)...);

        if (!result.valid())
        {
            [[maybe_unused]] const sol::error err = result;
            (void)CHECK(false, "Call to lua script function %s failed - %s", name.c_str(), err.what());
            return ELuaCallResult::FAILURE;
        }

        return ELuaCallResult::SUCCESS;
    }
}
