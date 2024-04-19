#pragma once
#include "PantheonScripting/LuaScript.h"

#include <PantheonCore/ECS/EntityHandle.h>
#include <PantheonCore/Resources/ResourceRef.h>

#include <cstdint>

#include <sol/sol.hpp>

namespace PantheonScripting
{
    enum class ELuaCallResult : uint8_t
    {
        INVALID_STATE,
        NOT_FOUND,
        SUCCESS,
        FAILURE
    };

    class LuaContext
    {
    public:
        struct ScriptHandle
        {
            PantheonCore::Resources::ResourceRef<LuaScript> m_script;
            PantheonCore::ECS::EntityHandle                 m_owner;
            sol::table                                      m_table = sol::nil;

            /**
             * \brief Checks whether this script handle should be ordered before the given one or not
             * \param other The script handle to compare against
             * \return True if this script handle should be ordered before the given one. False otherwise
             */
            bool operator<(const ScriptHandle& other) const;

            /**
             * \brief Checks whether the given script handle references the same script as this one or not
             * \param other The script handle to compare against
             * \return True if the given script handle references the same script as this one. False otherwise
             */
            bool operator==(const ScriptHandle& other) const;

            /**
             * \brief Checks whether the script handle is valid or not
             */
            operator bool() const;
        };

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
         * \param handle The script to register to the context
         * \return True on success. False otherwise
         */
        bool registerScript(ScriptHandle& handle);

        /**
         * \brief Adds the given script to the lua context
         * \param script The script to add to the context
         * \param owner The added script's owner
         * \param hint The added script's base table
         * \return A handle to the added script on success. An empty handle otherwise
         */
        ScriptHandle addScript(const std::string& script, const PantheonCore::ECS::EntityHandle& owner, const sol::table& hint);

        /**
         * \brief Gets a handle to the given script owned by the given entity
         * \param script The script to get a handle for
         * \param owner The script's owner
         * \return A handle to the found script. An empty handle if the script wasn't found
         */
        ScriptHandle getScript(const std::string& script, const PantheonCore::ECS::EntityHandle& owner) const;

        /**
         * \brief Removes the given script from the lua context
         * \param script The script to remove
         * \param owner The removed script's owner
         */
        void removeScript(const std::string& script, PantheonCore::ECS::EntityHandle& owner);

        /**
         * \brief Calls the function with the given name and parameters on the given lua object
         * \tparam Args The function parameter types
         * \param table The lua object on which the function should be called
         * \param name The function's name
         * \param args The function's parameters
         * \return The lua call result
         */
        template <typename... Args>
        ELuaCallResult tryCall(sol::table& table, const std::string& name, Args&&... args);

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
         * \param deltaTime The fixed elapsed time since the last fixed update
         */
        void fixedUpdate(float deltaTime);

        /**
         * \brief Stops all the registered scripts
         */
        void stop();

        /**
         * \brief Gets the current native lua state
         * \return The current native lua state
         */
        lua_State* getLuaState() const;

        /**
         * \brief Gets the given module's name
         * \param module The target module
         * \return The module's name
         */
        static const std::string& getModuleName(std::string module);

        /**
         * \brief Gets the given module's path
         * \param module The target module
         * \return The module's path
         */
        static const std::string& getModulePath(std::string module);

    private:
        static constexpr const char* EXTENSIONS[] = { ".lua", ".lc" };

        inline static std::unordered_map<std::string, std::string> s_moduleNames;
        inline static std::unordered_map<std::string, std::string> s_modulePaths;

        std::unique_ptr<sol::state> m_state;
        std::vector<ScriptHandle>   m_scripts;

        bool m_isValid, m_hasStarted;

        /**
         * \brief Loads a module from the given lua state
         * \param L The calling lua state
         * \return The number of elements left in the stack
         */
        static int loadModule(lua_State* L);

        /**
         * \brief Binds the necessary custom types
         * \param luaState The lua state to bind to
         */
        static void bindUserTypes(sol::state& luaState);
    };
}

#include "PantheonScripting/LuaContext.inl"
