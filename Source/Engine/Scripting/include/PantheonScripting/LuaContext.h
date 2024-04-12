#pragma once
#include "PantheonScripting/LuaScriptComponent.h"

#include <PantheonCore/ECS/EntityHandle.h>

#include <sol/sol.hpp>

namespace PantheonScripting
{
    class LuaContext
    {
    public:
        /**
         * \brief Creates a lua context
         */
        LuaContext();

        /**
         * \brief Disable lua context copying
         */
        LuaContext(const LuaContext&) = delete;

        /**
         * \brief Creates a move copy of the given lua context
         * \param other The lua context to move
         */
        LuaContext(LuaContext&& other) noexcept = default;

        /**
         * \brief Destroys the lua context
         */
        ~LuaContext();

        /**
         * \brief Disable lua context copying
         */
        LuaContext& operator=(const LuaContext&) = delete;

        /**
         * \brief Moves the given lua context into this one
         * \param other The lua context to move
         * \return A reference to the modified lua context
         */
        LuaContext& operator=(LuaContext&& other) noexcept = default;

        /**
         * \brief Initializes the lua context
         */
        void init();

        /**
         * \brief Resets the lua context
         */
        void reset();

        /**
         * \brief Checks whether the context is in a valid state or not
         * \return True if the context is in a valid state. False otherwise
         */
        bool isValid() const;

        /**
         * \brief Registers the given script to the lua context
         * \param entity The registered script's owner
         * \param script The script to register to the context
         * \return True on success. False otherwise
         */
        bool registerScript(const PantheonCore::ECS::EntityHandle& entity, LuaScriptComponent& script);

        /**
         * \brief Adds the given script to the lua context
         * \param entity The added script's owner
         * \param script The script to add to the context
         * \return True on success. False otherwise
         */
        bool addScript(const PantheonCore::ECS::EntityHandle& entity, LuaScriptComponent& script);

        /**
         * \brief Adds the given script to the lua context
         * \param entity The removed script's owner
         */
        void removeScript(PantheonCore::ECS::EntityHandle& entity);

        /**
         * \brief Calls the function with the given name and parameters on the given script
         * \tparam Args The function parameter types
         * \param component The script on which the function should be called
         * \param name The function's name
         * \param args The function's parameters
         * \return True if the function could be called. False otherwise
         */
        template <typename... Args>
        bool tryCall(LuaScriptComponent& component, const std::string& name, Args&&... args);

        /**
         * \brief Initializes and starts all the registered scripts
         */
        void start();

        /**
         * \brief Updates all the registered scripts every frame
         * \param deltaTime The elapsed time since the last update
         */
        void update(float deltaTime);

        /**
         * \brief Updates all the registered scripts at a fixed interval
         * \param fixedDeltaTime The elapsed time since the last update
         */
        void fixedUpdate(float fixedDeltaTime);

        /**
         * \brief Stops all the registered scripts
         */
        void stop();

        /**
         * \brief Gets the current native lua state
         * \return The current native lua state
         */
        lua_State* getLuaState();

    private:
        using ScriptHandle = std::pair<LuaScript::OrderT, PantheonCore::ECS::EntityHandle>;

        std::unique_ptr<sol::state> m_state;
        std::vector<ScriptHandle>   m_scripts;

        bool m_isValid, m_hasStarted;

        /**
         * \brief Loads a module from the given lua state
         * \param L The calling lua state
         * \return The number of elements left in the stack
         */
        static int loadModule(lua_State* L);
    };
}

#include "PantheonScripting/LuaContext.inl"
