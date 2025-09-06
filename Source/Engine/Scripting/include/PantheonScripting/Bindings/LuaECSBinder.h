#pragma once

namespace sol
{
    class state;
}

namespace PantheonScripting::Bindings
{
    class LuaECSBinder final
    {
    public:
        /**
         * \brief Creates or updates the ECS bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bind(sol::state& luaState);

    private:
        /**
         * \brief Creates or updates the entity bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindEntity(sol::state& luaState);
    };
}
