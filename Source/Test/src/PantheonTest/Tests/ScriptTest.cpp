#include "PantheonTest/Tests/ScriptTest.h"

#include "PantheonCore/Utility/Timer.h"

using namespace PantheonScripting;

using namespace PantheonCore::ECS;
using namespace PantheonCore::Utility;
using namespace PantheonCore::Resources;

namespace PantheonTest
{
    ScriptTest::ScriptTest(const size_t testScriptsCount)
        : ScriptTest("Script", testScriptsCount)
    {
    }

    ScriptTest::ScriptTest(const std::string& name, const size_t testScriptsCount)
        : ITest(name), m_luaContext(std::make_unique<LuaContext>()), m_testScriptsCount(testScriptsCount)
    {
        // TODO: Uncomment script test auto completion when ECS bindings are implemented
        // m_scene.getStorage<LuaScriptComponent>().m_onRemove.subscribe([this](Entity, LuaScriptComponent&)
        // {
        //     if (++m_destroyedScriptsCount == m_testScriptsCount)
        //     {
        //         stop();
        //         complete();
        //     }
        // });

        m_luaContext->init();

        ServiceLocator::provide<LuaContext>(*m_luaContext);
    }

    ScriptTest::~ScriptTest()
    {
        m_luaContext->reset();
    }

    void ScriptTest::onStart()
    {
        if (m_testScriptsCount == 0)
        {
            complete();
            return;
        }

        const LuaScriptComponent* testComponent = nullptr;
        ResourceRef<LuaScript>    scriptRef("4a4434b3-b0b3-493f-a55b-8241fe33cfd5", "scripts/test.lua");

        for (size_t i = 0; i < m_testScriptsCount; ++i)
        {
            EntityHandle entity                = m_scene.create();
            m_toSerialized[entity.getEntity()] = Entity(i);

            testComponent = &entity.make<LuaScriptComponent>(scriptRef);
        }

        testJsonSerialization(*testComponent);

        m_luaContext->start();
    }

    void ScriptTest::onUpdate()
    {
        m_luaContext->update(PTH_SERVICE(Timer).getDeltaTime());
    }

    void ScriptTest::onPostUpdate()
    {
        // TODO: Remove frame count based script test completion when ECS bindings are implemented
        if (PTH_SERVICE(Timer).getFrameCount() > 100)
        {
            stop();
            complete();
        }
    }

    void ScriptTest::onFixedUpdate()
    {
        m_luaContext->fixedUpdate(PTH_SERVICE(Timer).getFixedDeltaTime());
    }

    void ScriptTest::onStop()
    {
        m_luaContext->stop();
        m_scene.clear();
    }

    void ScriptTest::testJsonSerialization(const LuaScriptComponent& testComponent)
    {
        rapidjson::StringBuffer buffer;
        rapidjson::Writer       writer(buffer);
        TEST_CHECK(ComponentRegistry::toJson(testComponent, writer, m_toSerialized), "Script json serialization failed");
        TEST_CHECK(writer.IsComplete(), "Script json serialization failed - Produced json is incomplete");

        const std::string validJsonStr(buffer.GetString(), buffer.GetSize());

        const std::string invalidJsonStr = R"(
{
  "script": {
    "key": "INVALID",
    "path": "INVALID"
  },
  "data": [
    {
      "key": {
        "type": 4,
        "data": "owner"
      },
      "value": {
        "type": 3,
        "data": "SHOULD BE A NUMBER"
      }
    }
  ]
})";

        rapidjson::Document validJson;
        validJson.Parse(validJsonStr.c_str(), validJsonStr.size());

        rapidjson::Document invalidJson;
        invalidJson.Parse(invalidJsonStr.c_str(), invalidJsonStr.size());

        LuaScriptComponent script;
        TEST_CHECK(!ComponentRegistry::fromJson(script, invalidJson),
            "Script deserialization from invalid json should have failed");

        TEST_CHECK(ComponentRegistry::fromJson(script, validJson), "Script deserialization from valid json failed");

        m_scene.create().set(script);
    }
}
