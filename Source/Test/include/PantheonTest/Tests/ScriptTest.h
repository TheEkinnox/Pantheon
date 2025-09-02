#pragma once
#include "PantheonTest/Tests/ITest.h"

#include <PantheonCore/ECS/Scene.h>

namespace PantheonScripting
{
    class LuaContext;
    class LuaScriptList;
}

namespace PantheonTest
{
    class ScriptTest final : public ITest
    {
    public:
        explicit ScriptTest(size_t testScriptsCount = 250);
        explicit ScriptTest(const std::string& name, size_t testScriptsCount = 250);

    protected:
        void onStart() override;
        void onUpdate() override;
        void onFixedUpdate() override;
        void onPostUpdate() override;
        void onStop() override;

    private:
        std::unique_ptr<PantheonScripting::LuaContext> m_luaContext;

        PantheonCore::ECS::Scene                          m_scene;
        PantheonCore::ECS::ComponentRegistry::EntitiesMap m_toSerialized;

        size_t m_testScriptsCount;
        size_t m_destroyedScriptsCount;

        void testModuleUtility();
        void testJsonSerialization(const PantheonScripting::LuaScriptList& testScriptList);
        void testBinarySerialization(const PantheonScripting::LuaScriptList& testScriptList);
    };
}
