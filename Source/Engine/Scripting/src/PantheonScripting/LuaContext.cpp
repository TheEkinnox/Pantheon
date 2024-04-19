#include "PantheonScripting/LuaContext.h"

#include "PantheonCore/Utility/FileSystem.h"

#include "PantheonScripting/FunctionNames.h"
#include "PantheonScripting/LuaScriptList.h"
#include "PantheonScripting/Bindings/LuaECSBinder.h"
#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include <PantheonCore/Resources/ResourceManager.h>
#include <PantheonCore/Utility/ServiceLocator.h>

using namespace PantheonCore::ECS;
using namespace PantheonCore::Resources;
using namespace PantheonCore::Utility;

namespace PantheonScripting
{
    bool LuaContext::ScriptHandle::operator<(const ScriptHandle& other) const
    {
        return (m_script && other.m_script && m_script->getExecutionOrder() < other.m_script->getExecutionOrder())
            || m_script.getKey().compare(other.m_script.getKey()) < 0
            || m_script.getPath().compare(other.m_script.getPath()) < 0
            || m_owner.getEntity() < other.m_owner.getEntity()
            || std::less<Scene*>{}(m_owner.getScene(), other.m_owner.getScene());
    }

    bool LuaContext::ScriptHandle::operator==(const ScriptHandle& other) const
    {
        return m_owner == other.m_owner && m_script == other.m_script;
    }

    LuaContext::ScriptHandle::operator bool() const
    {
        if (!m_owner || !m_script)
            return false;

        const LuaScriptList* scripts = m_owner.get<LuaScriptList>();
        return scripts->contains(m_script.getKey());
    }

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
        if (ASSUME_FALSE(m_state, "Attempted to initialize an already initialized lua context."))
            return;

        m_state = std::make_unique<sol::state>();
        m_state->open_libraries(sol::lib::base, sol::lib::package, sol::lib::math);
        m_state->add_package_loader(&loadModule);
        m_isValid = true;

        bindUserTypes(*m_state);

        for (size_t i = m_scripts.size(); i > 0; --i)
        {
            if (i > m_scripts.size())
                continue;

            EntityHandle   entity = m_scripts[i - 1].m_owner;
            LuaScriptList* script = entity.get<LuaScriptList>();

            if (script && !registerScript(m_scripts[i - 1]))
                return;
        }
    }

    void LuaContext::reset()
    {
        for (size_t i = m_scripts.size(); i > 0 && m_isValid; --i)
        {
            if (i > m_scripts.size())
                continue;

            m_isValid = tryCall(m_scripts[i - 1].m_table, ScriptingFunctions::DESTROY) != ELuaCallResult::FAILURE;
            ASSERT(m_isValid);

            m_scripts[i - 1].m_table = sol::nil;
        }

        if (m_state)
            m_state.reset();

        m_isValid = false;
    }

    bool LuaContext::isValid() const
    {
        return m_isValid;
    }

    bool LuaContext::registerScript(ScriptHandle& handle)
    {
        if (!CHECK(isValid(), "Attempted to register script \"%s\" to invalid lua context", handle.m_script.getPath().c_str()))
            return false;

        if (!CHECK(handle.m_script, "Attempted to register unloaded script \"%s\"", handle.m_script.getPath().c_str()))
            return (m_isValid = false);

        const std::string_view source = handle.m_script->getSource();

        if (source.empty())
            return (m_isValid = false);

        const auto result = m_state->safe_script(source, &sol::script_pass_on_error);

        if (!result.valid())
        {
            [[maybe_unused]] const sol::error err = result;
            CHECK(false, "Failed to register script \"%s\" - %s", handle.m_script.getPath().c_str(), err.what());
            return (m_isValid = false);
        }

        if (!CHECK(result.return_count() == 1 && result[0].is<sol::table>(),
                "Failed to register script \"%s\" - Invalid return", handle.m_script.getPath().c_str()))
            return (m_isValid = false);

        sol::table resultTable = result[0].as<sol::table>();

        if (handle.m_table != sol::nil)
            for (auto& [key, value] : handle.m_table)
                resultTable[key] = value;

        resultTable["owner"] = static_cast<const EntityHandle&>(handle.m_owner);

        if (tryCall(resultTable, ScriptingFunctions::INIT) == ELuaCallResult::FAILURE)
            return (m_isValid = false);

        if (m_hasStarted && tryCall(resultTable, ScriptingFunctions::START) == ELuaCallResult::FAILURE)
            return (m_isValid = false);

        handle.m_table = resultTable;
        return m_isValid;
    }

    LuaContext::ScriptHandle LuaContext::addScript(const std::string& script, const EntityHandle& owner, const sol::table& hint)
    {
        const ResourceRef<LuaScript> scriptRef(getModuleName(script), getModulePath(script));

        ScriptHandle handle = { scriptRef, owner, hint };

        if (!registerScript(handle))
            return {};

        const auto insertIt = std::ranges::find_if_not(m_scripts, [&handle](const ScriptHandle& other)
        {
            return handle < other;
        });

        m_isValid = CHECK(m_scripts.insert(insertIt, handle) != m_scripts.end(), "Failed to add script \"%s\"", script.c_str());

        return m_isValid ? handle : ScriptHandle{};
    }

    LuaContext::ScriptHandle LuaContext::getScript(const std::string& script, const EntityHandle& owner) const
    {
        const auto& moduleName = getModuleName(script);
        const auto& modulePath = getModulePath(script);

        const auto it = std::ranges::find_if(m_scripts, [&moduleName, &modulePath, &owner](const ScriptHandle& other)
        {
            return (other.m_script.getKey() == moduleName || other.m_script.getPath() == modulePath)
                && other.m_owner.getEntity() == owner.getEntity()
                && other.m_owner.getScene() == owner.getScene();
        });

        return it != m_scripts.end() ? *it : ScriptHandle{};
    }

    void LuaContext::removeScript(const std::string& script, EntityHandle& owner)
    {
        const auto& moduleName = getModuleName(script);
        const auto& modulePath = getModulePath(script);

        const auto it = std::ranges::find_if(m_scripts, [&moduleName, &modulePath, &owner](const ScriptHandle& other)
        {
            return (other.m_script.getKey() == moduleName || other.m_script.getPath() == modulePath)
                && other.m_owner.getEntity() == owner.getEntity()
                && other.m_owner.getScene() == owner.getScene();
        });

        if (it != m_scripts.end())
        {
            m_isValid = tryCall(it->m_table, ScriptingFunctions::DESTROY) != ELuaCallResult::FAILURE;

            it->m_table = sol::nil;
            m_scripts.erase(it);
        }
    }

    void LuaContext::start()
    {
        if (ASSUME_FALSE(m_hasStarted, "Attempted to recall start function on started lua context."))
            return;

        for (size_t i = m_scripts.size(); i > 0 && m_isValid; --i)
        {
            if (i <= m_scripts.size())
                m_isValid = tryCall(m_scripts[i - 1].m_table, ScriptingFunctions::START) != ELuaCallResult::FAILURE;
        }

        m_hasStarted = m_isValid;
    }

    void LuaContext::update(const float deltaTime)
    {
        for (size_t i = m_scripts.size(); i > 0 && m_isValid; --i)
        {
            if (i <= m_scripts.size())
                m_isValid = tryCall(m_scripts[i - 1].m_table, ScriptingFunctions::UPDATE, deltaTime) != ELuaCallResult::FAILURE;
        }
    }

    void LuaContext::fixedUpdate(const float deltaTime)
    {
        for (size_t i = m_scripts.size(); i > 0 && m_isValid; --i)
        {
            if (i > m_scripts.size())
                continue;

            m_isValid = tryCall(m_scripts[i - 1].m_table, ScriptingFunctions::FIXED_UPDATE, deltaTime) != ELuaCallResult::FAILURE;
        }
    }

    void LuaContext::stop()
    {
        if (!ASSUME(m_hasStarted, "Attempted to stop a non started lua context"))
            return;

        for (size_t i = m_scripts.size(); i > 0 && m_isValid; --i)
        {
            if (i <= m_scripts.size())
                m_isValid = tryCall(m_scripts[i - 1].m_table, ScriptingFunctions::STOP) != ELuaCallResult::FAILURE;
        }

        m_hasStarted = false;
    }

    lua_State* LuaContext::getLuaState()
    {
        return m_state ? m_state->lua_state() : nullptr;
    }

    const std::string& LuaContext::getModuleName(std::string module)
    {
        static std::string empty;

        if (module.empty())
            return empty;

        const auto it = s_moduleNames.find(module);

        if (it != s_moduleNames.end())
            return it->second;

        const std::string base = module;

        for (const auto& extension : EXTENSIONS)
        {
            if (module.ends_with(extension))
                module = module.substr(0, module.size() - strlen(extension));
        }

        replaceInPlace(module, ".", "/");

        module = PTH_SERVICE(ResourceManager).getRelativePath(module);

        replaceInPlace(module, "/", ".");
        replaceInPlace(module, "\\", ".");

        return (s_moduleNames[base] = module);
    }

    const std::string& LuaContext::getModulePath(std::string module)
    {
        static std::string empty;

        if (module.empty())
            return empty;

        const auto it = s_modulePaths.find(module);

        if (it != s_modulePaths.end())
            return it->second;

        const std::string base = module;

        module = replace(getModuleName(module), ".", "/");

        const ResourceManager& resourceManager = PTH_SERVICE(ResourceManager);

        for (const auto& extension : EXTENSIONS)
        {
            std::string path(resourceManager.getFullPath(module + extension));

            if (pathExists(path))
                return (s_modulePaths[base] = path);
        }

        return empty;
    }

    int LuaContext::loadModule(lua_State* L)
    {
        const std::string& module = getModulePath(sol::stack::get<std::string>(L, 1));

        if (module.empty())
            return 1;

        const std::vector<char> script = PTH_SERVICE(ResourceManager).readFile(module);

        if (!script.empty() && luaL_loadbuffer(L, script.data(), script.size(), module.c_str()) == LUA_OK)
        {
            sol::stack::push(L, getModuleName(module).c_str());
            return 2;
        }

        return 1;
    }

    void LuaContext::bindUserTypes(sol::state& luaState)
    {
        Bindings::LuaECSBinder::bind(luaState);
        Bindings::LuaMathBinder::bind(luaState);
    }
}
