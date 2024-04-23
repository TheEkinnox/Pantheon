#include "PantheonScripting/LuaScriptHandle.h"

#include "PantheonScripting/LuaScriptList.h"

using namespace PantheonCore::ECS;

namespace PantheonScripting
{
    bool LuaScriptHandle::operator<(const LuaScriptHandle& other) const
    {
        return (m_script && other.m_script && m_script->getExecutionOrder() < other.m_script->getExecutionOrder())
            || m_script.getKey().compare(other.m_script.getKey()) < 0
            || m_script.getPath().compare(other.m_script.getPath()) < 0
            || m_owner.getEntity() < other.m_owner.getEntity()
            || std::less<Scene*>{}(m_owner.getScene(), other.m_owner.getScene());
    }

    bool LuaScriptHandle::operator==(const LuaScriptHandle& other) const
    {
        return m_owner == other.m_owner && m_script == other.m_script;
    }

    LuaScriptHandle::operator bool() const
    {
        if (!m_owner || !m_script)
            return false;

        const LuaScriptList* scripts = m_owner.get<LuaScriptList>();
        return scripts->contains(m_script.getKey());
    }
}
