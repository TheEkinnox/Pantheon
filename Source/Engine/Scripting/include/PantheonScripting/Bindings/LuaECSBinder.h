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
         * \brief Creates or updates the tag bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindTag(sol::state& luaState);

        /**
         * \brief Creates or updates the scene bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindScene(sol::state& luaState);

        /**
         * \brief Creates or updates the entity bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindEntity(sol::state& luaState);

        /**
         * \brief Creates or updates the component bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindComponent(sol::state& luaState);

        /**
         * \brief Creates or updates the script handle bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindScriptHandle(sol::state& luaState);

        /**
         * \brief Creates or updates the component search origin bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindComponentSearchOrigin(sol::state& luaState);
    };
}
