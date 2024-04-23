#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"
#include "PantheonCore/ECS/EntityHandle.h"

namespace PantheonScripting
{
    struct LuaComponentHandle
    {
        PantheonCore::ECS::EntityHandle              m_owner;
        PantheonCore::ECS::ComponentRegistry::TypeId m_typeId;

        /**
         * \brief Checks whether the given handle references the same component as this one or not
         * \param other The component handle to compare against
         * \return True if the given handle references the same component. False otherwise
         */
        bool operator==(const LuaComponentHandle& other) const;

        /**
         * \brief Checks whether the component handle is valid or not
         */
        operator bool() const;
    };
}
