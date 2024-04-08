#include "PantheonScripting/LuaContext.h"

#include "PantheonScripting/FunctionNames.h"
#include "PantheonScripting/LuaScriptComponent.h"

#include <PantheonCore/Resources/ResourceManager.h>
#include <PantheonCore/Utility/ServiceLocator.h>

using namespace PantheonCore::ECS;
using namespace PantheonCore::Resources;
using namespace PantheonCore::Utility;

namespace PantheonScripting
{
    LuaContext::LuaContext()
        : m_isValid(false), m_hasStarted(false)
    {
    }

    LuaContext::~LuaContext()
    {
        reset();
    }

    void LuaContext::init()
    {
        if (!ASSUME(!m_state, "Attempted to initialize an already initialized lua context."))
            return;

        lua_CFunction loadModule = [](lua_State* L)
        {
            const std::string path = std::filesystem::path(sol::stack::get<std::string>(L, 1)).replace_extension(".lua").string();

            const std::vector<char> script = PTH_SERVICE(ResourceManager).readFile(path);

            if (!script.empty() && luaL_loadbuffer(L, script.data(), script.size(), path.c_str()) == LUA_OK)
            {
                sol::stack::push(L, path.c_str());
                return 2;
            }

            return 1;
        };

        m_state = std::make_unique<sol::state>();
        m_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::string, sol::lib::math, sol::lib::table);
        m_state->add_package_loader(loadModule);
        m_isValid = true;
    }

    void LuaContext::reset()
    {
        if (m_state)
            m_state.reset();

        m_scripts.clear();
        m_isValid = false;
    }

    bool LuaContext::isValid() const
    {
        return m_isValid;
    }

    bool LuaContext::addScript(EntityHandle& entity, LuaScriptComponent& script)
    {
        [[maybe_unused]] const char* path = script.m_script.getPath().c_str();

        if (!CHECK(isValid(), "Attempted to add script %s to invalid lua context", path))
            return false;

        const LuaScript* luaScript = script.m_script.get();

        if (!CHECK(luaScript, "Attempted to add unloaded script %s", path))
            return (m_isValid = false);

        const std::string_view source = luaScript->getSource();

        if (source.empty())
            return (m_isValid = false);

        const auto result = m_state->safe_script(source, &sol::script_pass_on_error);

        if (!result.valid())
        {
            [[maybe_unused]] const sol::error err = result;
            CHECK(false, "Failed to add script %s - %s", path, err.what());
            return (m_isValid = false);
        }

        if (!CHECK(result.return_count() == 1 && result[0].is<sol::table>(), "Failed to add script %s - Invalid return", path))
            return (m_isValid = false);

        const LuaScript::OrderT executionOrder = luaScript->getExecutionOrder();

        const auto insertIt = std::ranges::find_if_not(m_scripts, [&entity, executionOrder](const ScriptHandle& other)
        {
            return executionOrder < other.first
                || entity.getEntity() < other.second.getEntity()
                || reinterpret_cast<size_t>(entity.getScene()) < reinterpret_cast<size_t>(other.second.getScene());
        });

        if (!CHECK(m_scripts.emplace(insertIt, executionOrder, entity) != m_scripts.end(), "Failed to add script %s", path))
            return (m_isValid = false);

        script.m_table          = result[0];
        script.m_table["owner"] = static_cast<Entity::Id>(entity.getEntity());

        tryCall(script, ScriptingFunctions::INIT);

        if (m_hasStarted)
            tryCall(script, ScriptingFunctions::START);

        return (m_isValid = true);
    }

    void LuaContext::removeScript(EntityHandle& entity)
    {
        const auto it = std::ranges::find_if(m_scripts, [&entity](const ScriptHandle& other)
        {
            return other.second.getEntity() == entity.getEntity() && other.second.getScene() == entity.getScene();
        });

        if (it != m_scripts.end())
        {
            LuaScriptComponent* scriptComponent = entity.get<LuaScriptComponent>();

            if (scriptComponent)
                tryCall(*scriptComponent, ScriptingFunctions::DESTROY);

            m_scripts.erase(it);
        }
    }

    void LuaContext::start()
    {
        if (!ASSUME(!m_hasStarted, "Attempted to recall start function on started lua context."))
            return;

        for (auto entity : m_scripts | std::views::values)
        {
            LuaScriptComponent& script = *entity.get<LuaScriptComponent>();
            tryCall(script, ScriptingFunctions::START);
        }

        m_hasStarted = true;
    }

    void LuaContext::update(const float deltaTime)
    {
        for (auto entity : m_scripts | std::views::values)
        {
            LuaScriptComponent& script = *entity.get<LuaScriptComponent>();
            tryCall(script, ScriptingFunctions::UPDATE, deltaTime);
        }
    }

    void LuaContext::fixedUpdate(const float fixedDeltaTime)
    {
        for (auto entity : m_scripts | std::views::values)
        {
            LuaScriptComponent& script = *entity.get<LuaScriptComponent>();
            tryCall(script, ScriptingFunctions::FIXED_UPDATE, fixedDeltaTime);
        }
    }

    void LuaContext::stop()
    {
        if (!ASSUME(m_hasStarted, "Attempted to stop a non started lua context"))
            return;

        for (auto entity : m_scripts | std::views::values)
        {
            LuaScriptComponent& script = *entity.get<LuaScriptComponent>();
            tryCall(script, ScriptingFunctions::STOP);
        }

        m_hasStarted = false;
    }
}
