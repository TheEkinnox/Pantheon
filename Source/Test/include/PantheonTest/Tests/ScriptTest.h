#pragma once
#include "ITest.h"

#include "PantheonScripting/LuaContext.h"

namespace PantheonTest
{
    class ScriptTest final : public ITest
    {
    public:
        explicit ScriptTest(size_t testScriptsCount = 250);
        explicit ScriptTest(const std::string& name, size_t testScriptsCount = 250);
        ~ScriptTest() override;

    protected:
        void onStart() override;
        void onUpdate() override;
        void onPostUpdate() override;
        void onFixedUpdate() override;
        void onStop() override;

    private:
        std::unique_ptr<PantheonScripting::LuaContext> m_luaContext;

        PantheonCore::ECS::Scene                          m_scene;
        PantheonCore::ECS::ComponentRegistry::EntitiesMap m_toSerialized;

        size_t m_testScriptsCount;
        size_t m_destroyedScriptsCount;

        void testJsonSerialization(const PantheonScripting::LuaScriptComponent& testComponent);
    };
}
