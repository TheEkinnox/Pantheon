#include "PantheonTest/Tests/ScriptTest.h"

#include <PantheonCore/Utility/Timer.h>

#include <PantheonScripting/LuaContext.h>
#include <PantheonScripting/LuaScriptList.h>

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
        : ITest(name), m_luaContext(std::make_unique<LuaContext>()), m_testScriptsCount(testScriptsCount),
        m_destroyedScriptsCount(0)
    {
        m_scene.getStorage<LuaScriptList>().m_onRemove.subscribe([this](Entity, LuaScriptList&)
        {
            ++m_destroyedScriptsCount;
        });

        ServiceLocator::provide<LuaContext>(*m_luaContext);
    }

    void ScriptTest::onStart()
    {
        m_luaContext->init();

        TEST_CHECK(m_luaContext->isValid());

        if (m_testScriptsCount == 0 || !m_luaContext->isValid())
        {
            complete();
            return;
        }

        testModuleUtility();

        LuaScriptList* testComponent = nullptr;

        for (size_t i = 0; i < m_testScriptsCount; ++i)
        {
            EntityHandle entity                = m_scene.create();
            m_toSerialized[entity.getEntity()] = Entity(i);

            testComponent = &entity.make<LuaScriptList>();
            testComponent->add("scripts/test.lua");

            TEST_CHECK(m_luaContext->isValid());

            if (!m_luaContext->isValid())
            {
                complete();
                return;
            }
        }

        testJsonSerialization(*testComponent);
        testBinarySerialization(*testComponent);

        m_luaContext->start();

        TEST_CHECK(m_luaContext->isValid());

        if (!m_luaContext->isValid())
            complete();
    }

    void ScriptTest::onUpdate()
    {
        m_luaContext->update(PTH_SERVICE(Timer).getDeltaTime());

        TEST_CHECK(m_luaContext->isValid());

        if (!m_luaContext->isValid())
            complete();
    }

    void ScriptTest::onFixedUpdate()
    {
        m_luaContext->fixedUpdate(PTH_SERVICE(Timer).getFixedDeltaTime());

        TEST_CHECK(m_luaContext->isValid());

        if (!m_luaContext->isValid())
            complete();
    }

    void ScriptTest::onPostUpdate()
    {
        if (m_destroyedScriptsCount >= m_testScriptsCount)
        {
            stop();
            complete();
        }
    }

    void ScriptTest::onStop()
    {
        m_luaContext->stop();

        TEST_CHECK(m_luaContext->isValid());

        if (!m_luaContext->isValid())
        {
            complete();
            return;
        }

        m_scene.clear();

        m_luaContext->reset();
    }

    void ScriptTest::testModuleUtility()
    {
        static const std::string modules[] =
        {
            "scripts.testRequired", "scripts.testRequired.lua", "scripts.testRequired.lc",
            "scripts/testRequired", "scripts/testRequired.lua", "scripts/testRequired.lc",
            "assets.scripts.testRequired", "assets.scripts.testRequired.lua", "assets.scripts.testRequired.lc",
            "assets.scripts/testRequired", "assets.scripts/testRequired.lua", "assets.scripts/testRequired.lc",
            "assets/scripts.testRequired", "assets/scripts.testRequired.lua", "assets/scripts.testRequired.lc",
            "assets/scripts/testRequired", "assets/scripts/testRequired.lua", "assets/scripts/testRequired.lc"
        };

        static constexpr const char* expectedName = "scripts.testRequired";
        static const std::string expectedPath = std::filesystem::path("assets/scripts/testRequired.lua").make_preferred().string();

        for (const auto& module : modules)
        {
            const std::string& name = LuaContext::getModuleName(module);
            const std::string& path = LuaContext::getModulePath(module);

            TEST_CHECK(name == expectedName, "Invalid module name \"%s\". Expected \"%s\"", name.c_str(), expectedName);
            TEST_CHECK(path == expectedPath, "Invalid module path \"%s\". Expected \"%s\"", path.c_str(), expectedPath.c_str());
        }
    }

    void ScriptTest::testJsonSerialization(const LuaScriptList& testComponent)
    {
        // TODO: Remove temporary json scene file creation
        {
            rapidjson::StringBuffer buffer;
            rapidjson::Writer       writer(buffer);
            TEST_CHECK(m_scene.toJson(writer), "Script json serialization failed");
            TEST_CHECK(writer.IsComplete(), "Script json serialization failed - Produced json is incomplete");

            const std::string validJsonStr(buffer.GetString(), buffer.GetSize());

            std::ofstream file("tmp.pthscene");
            file << validJsonStr;
        }

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

        LuaScriptList script;
        TEST_CHECK(!ComponentRegistry::fromJson(script, invalidJson, &m_scene),
            "Script deserialization from invalid json should have failed");

        TEST_CHECK(ComponentRegistry::fromJson(script, validJson, &m_scene), "Script deserialization from valid json failed");

        m_scene.create().set(script);
    }

    void ScriptTest::testBinarySerialization(const LuaScriptList& testComponent)
    {
        // TODO: Remove temporary binary scene file creation
        {
            std::vector<char> out;
            TEST_CHECK(m_scene.toBinary(out), "Script binary serialization failed");
            TEST_CHECK(!out.empty(), "Script binary serialization failed - Nothing was written in the output buffer");

            std::ofstream file("tmp.pthscene.bin");
            file.write(out.data(), static_cast<std::streamsize>(out.size()));
        }

        std::vector<char> validArray;
        TEST_CHECK(ComponentRegistry::toBinary(testComponent, validArray, m_toSerialized), "Script binary serialization failed");
        TEST_CHECK(!validArray.empty(), "Script binary serialization failed - Nothing was written in the output buffer");

        std::vector<char> invalidArray = validArray;
        invalidArray.resize(invalidArray.size() / 2);

        LuaScriptList script;
        TEST_CHECK(!ComponentRegistry::fromBinary(script, invalidArray.data(), invalidArray.size(), &m_scene),
            "Script deserialization from invalid memory buffer should have failed");

        TEST_CHECK(ComponentRegistry::fromBinary(script, validArray.data(), validArray.size(), &m_scene),
            "Script deserialization from valid memory buffer failed");

        m_scene.create().set(script);
    }
}
