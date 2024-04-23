#include "PantheonScripting/LuaComponentHandle.h"

namespace PantheonScripting
{
    bool LuaComponentHandle::operator==(const LuaComponentHandle& other) const
    {
        return m_typeId == other.m_typeId && m_owner == other.m_owner;
    }

    LuaComponentHandle::operator bool() const
    {
        return m_owner && m_owner.getScene()->getStorage(m_typeId).contains(m_owner);
    }
}
