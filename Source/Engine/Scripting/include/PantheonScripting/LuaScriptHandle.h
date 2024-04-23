#pragma once
#include "PantheonScripting/LuaScript.h"

#include <PantheonCore/ECS/EntityHandle.h>
#include <PantheonCore/Resources/ResourceRef.h>

#include <sol/table.hpp>

namespace PantheonScripting
{
    struct LuaScriptHandle
    {
        PantheonCore::Resources::ResourceRef<LuaScript> m_script;
        PantheonCore::ECS::EntityHandle                 m_owner;
        sol::table                                      m_table = sol::nil;

        /**
         * \brief Checks whether this script handle should be ordered before the given one or not
         * \param other The script handle to compare against
         * \return True if this script handle should be ordered before the given one. False otherwise
         */
        bool operator<(const LuaScriptHandle& other) const;

        /**
         * \brief Checks whether the given script handle references the same script as this one or not
         * \param other The script handle to compare against
         * \return True if the given script handle references the same script as this one. False otherwise
         */
        bool operator==(const LuaScriptHandle& other) const;

        /**
         * \brief Checks whether the script handle is valid or not
         */
        operator bool() const;
    };
}
