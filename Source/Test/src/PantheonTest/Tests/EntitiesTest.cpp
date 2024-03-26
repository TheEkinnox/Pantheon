#include "PantheonTest/Tests/EntitiesTest.h"

#include <PantheonCore/ECS/EntityStorage.h>
#include <PantheonCore/ECS/SceneView.h>
#include <PantheonCore/ECS/Components/Hierarchy.h>
#include <PantheonCore/ECS/Components/TagComponent.h>

using namespace LibMath;
using namespace PantheonCore::ECS;

namespace PantheonTest
{
    EntitiesTest::EntitiesTest()
        : EntitiesTest("Entities")
    {
    }

    EntitiesTest::EntitiesTest(const std::string& name)
        : ITest(name)
    {
    }

    void EntitiesTest::testScene()
    {
        const auto onAddEntity = [](const Entity p_entity)
        {
            DEBUG_LOG("Created entity %d:%d", p_entity.getIndex(), p_entity.getVersion());
        };

        const auto onRemoveEntity = [](const EntityHandle p_entity)
        {
            for (auto child : p_entity.getChildren())
                child.destroy();

            DEBUG_LOG("Removed entity %d:%d", p_entity.getEntity().getIndex(), p_entity.getEntity().getVersion());
        };

        const auto onAddInt = [](const Entity p_owner, const int& p_val)
        {
            DEBUG_LOG("Added int %d to entity %d:%d", p_val, p_owner.getIndex(), p_owner.getVersion());
        };

        const auto onChangeInt = [](const Entity p_owner, const int& p_val)
        {
            DEBUG_LOG("Changed int of entity %d:%d to %d", p_owner.getIndex(), p_owner.getVersion(), p_val);
        };

        const auto onRemoveInt = [](const Entity p_owner, const int& p_val)
        {
            DEBUG_LOG("Removed int %d from entity %d:%d", p_val, p_owner.getIndex(), p_owner.getVersion());
        };

        Scene scene;

        scene.getStorage<Entity>().m_onAdd.subscribe(onAddEntity);
        scene.getStorage<Entity>().m_onRemove.subscribe(onRemoveEntity);

        scene.getStorage<int>().m_onAdd.subscribe(onAddInt);
        scene.getStorage<int>().m_onChange.subscribe(onChangeInt);
        scene.getStorage<int>().m_onRemove.subscribe(onRemoveInt);

        scene.make<int>(scene.create(), 0);
        scene.make<int>(scene.create(), 1);
        scene.make<int>(scene.create(), 2);
        scene.make<int>(scene.create(), 3);

        EntityHandle lastEntity = scene.create();
        scene.make<int>(lastEntity, 999);
        scene.set<int>(lastEntity, 4);

        TEST_CHECK(scene.getStorage<Entity>().getCount() == 5);
        TEST_CHECK(scene.getStorage<int>().getCount() == 5);

        scene.remove<const int>(Entity(1));
        TEST_CHECK(scene.getStorage<int>().getCount() == 4);

        scene.remove<int>(*scene.get<int>(lastEntity));
        TEST_CHECK(scene.getStorage<int>().getCount() == 3);

        scene.remove<int>(lastEntity);
        TEST_CHECK(scene.getStorage<int>().getCount() == 3);

        SceneView<int, char, float> view(scene);

        lastEntity.set<int>(8);
        lastEntity.set<char>('j');
        lastEntity.set<float>(2.5f);

        TEST_CHECK(scene.getStorage<int>().getCount() == 4);
        TEST_CHECK(scene.getStorage<float>().getCount() == 1);
        TEST_CHECK(scene.getStorage<char>().getCount() == 1);

        const char c1       = *view.get<char>(lastEntity);
        auto       [i1, f1] = view.get<int, float>(lastEntity);

        TEST_CHECK(c1 == 'j');
        TEST_CHECK(i1 && *i1 == 8);
        TEST_CHECK(f1 && floatEquals(*f1, 2.5f));

        auto [i, c, f] = view.get(lastEntity);

        TEST_CHECK(i && *i == 8);
        TEST_CHECK(c && *c == 'j');
        TEST_CHECK(f && floatEquals(*f, 2.5f));

        int count = 0;
        for (const auto entity : view)
        {
            TEST_CHECK(++count == 1, "Only one relevant entity should have been found");

            auto [iComp, fComp, cComp] = view.get<int, const float, char>(entity);

            TEST_CHECK(iComp && *iComp == 8);
            TEST_CHECK(cComp && *cComp == 'j');
            TEST_CHECK(fComp && floatEquals(*fComp, 2.5f));

            *iComp = 123;
            *cComp = 'z';

            std::cout << "iComp: " << *iComp << " | cComp: " << *cComp << " | fComp: " << *fComp << '\n';
        }

        auto [i2, c2, f2] = view.get(lastEntity);

        TEST_CHECK(i2 && *i2 == 123);
        TEST_CHECK(c2 && *c2 == 'z');
        TEST_CHECK(f2 && floatEquals(*f2, 2.5f));

        lastEntity.destroy();

        TEST_CHECK(scene.getStorage<Entity>().getCount() == 4);
        TEST_CHECK(scene.getStorage<int>().getCount() == 3);
        TEST_CHECK(scene.getStorage<float>().getCount() == 0);
        TEST_CHECK(scene.getStorage<char>().getCount() == 0);

        auto [iDestroyed, cDestroyed, fDestroyed] = view.get(lastEntity);

        TEST_CHECK(iDestroyed == nullptr);
        TEST_CHECK(cDestroyed == nullptr);
        TEST_CHECK(fDestroyed == nullptr);

        for (const auto _ : view)
        {
            TEST_CHECK(false, "The only releveant entity should have been destroyed");
        }

        lastEntity = scene.create();

        TEST_CHECK(scene.getStorage<Entity>().getCount() == 5);
        TEST_CHECK(scene.getStorage<int>().getCount() == 3);
        TEST_CHECK(scene.getStorage<float>().getCount() == 0);
        TEST_CHECK(scene.getStorage<char>().getCount() == 0);

        for (const auto _ : view)
        {
            TEST_CHECK(false, "The only releveant entity should have been destroyed");
        }
    }

    Scene EntitiesTest::makeScene()
    {
        Scene scene;
        scene.clear();

        EntityHandle entity1 = scene.create();
        entity1.make<TagComponent>("Entity 1");
        entity1.make<Transform>(Vector3::zero(), Quaternion::identity(), Vector3::one());

        EntityHandle entity2 = scene.create();
        entity2.make<TagComponent>("Entity 2");
        entity2.make<Transform>(Vector3::zero(), Quaternion::identity(), Vector3::one());

        EntityHandle entity3 = scene.create();
        entity3.make<TagComponent>("Entity 3");
        entity3.make<Transform>(Vector3(.75f, .5f, 0.f), Quaternion::identity(), Vector3::one());

        EntityHandle entity4 = scene.create();
        entity4.make<TagComponent>("Entity 4 (1st child of Entity 3)");
        entity4.make<HierarchyComponent>(entity3.getEntity());
        entity4.make<Transform>(Vector3(0.f, 1.5f, 0.f), Quaternion::identity(), Vector3::one());

        EntityHandle entity5 = scene.create();
        entity5.make<TagComponent>("Entity 5 (1st child of Entity 4)");
        entity5.make<HierarchyComponent>(entity4.getEntity());
        entity5.make<Transform>(Vector3(0.f, 1.5f, 0.f), Quaternion::identity(), Vector3::one());

        EntityHandle entity6 = scene.create();
        entity6.make<TagComponent>("Entity 6 (2nd child of Entity 4)");
        entity6.make<HierarchyComponent>(entity4.getEntity());
        entity6.make<Transform>(Vector3(1.5f, 1.5f, 0.f), Quaternion::identity(), Vector3::one());

        EntityHandle entity7 = scene.create();
        entity6.make<TagComponent>("Entity 7 (2nd child of Entity 3)");
        entity7.make<HierarchyComponent>(entity3.getEntity());
        entity7.make<Transform>(Vector3(-1.5f, 1.5f, 0.f), Quaternion::identity(), Vector3::one());

        EntityHandle entity8 = scene.create();
        entity8.make<TagComponent>("Entity 8");
        entity8.make<Transform>(Vector3::front(), Quaternion::identity(), Vector3(1.5f, .5f, .1f));

        return scene;
    }

    void EntitiesTest::onStart()
    {
        testScene();
        testComponents();
        testJsonSerialization();
        testBinarySerialization();

        complete();
    }

    void EntitiesTest::testEntityStorage()
    {
        EntityStorage entityStorage;

        TEST_CHECK(entityStorage.add() == Entity(0, 0));
        TEST_CHECK(entityStorage.add() == Entity(1, 0));
        TEST_CHECK(entityStorage.add() == Entity(2, 0));

        TEST_CHECK(entityStorage.has(Entity(0, 0)));
        TEST_CHECK(entityStorage.has(Entity(1, 0)));
        TEST_CHECK(entityStorage.has(Entity(2, 0)));

        entityStorage.remove(Entity(1, 0));
        TEST_CHECK(!entityStorage.has(Entity(1, 0)));
        TEST_CHECK(!entityStorage.has(Entity(1, 0)));
    }

    void EntitiesTest::testComponentStorage()
    {
        ComponentStorage<char> storage;

        storage.construct(Entity(0), '0');
        storage.construct(Entity(1), '1');
        storage.construct(Entity(2), '2');
        storage.construct(Entity(3), '3');
        storage.construct(Entity(4), '4');

        uint8_t i = 0;

        for (const char c : storage)
        {
            TEST_CHECK(c == static_cast<char>('0' + i++));
        }

        storage.remove(Entity(1));
        storage.remove(*storage.find(Entity(4)));
    }

    void EntitiesTest::testComponents()
    {
        Scene        scene;
        EntityHandle entity = scene.create();

        const Transform&    transform = entity.make<Transform>();
        const TagComponent& tag       = entity.make<TagComponent>("Test");

        TEST_CHECK(entity.get<Transform>() == &transform, "get and addComponent should return the same address");
        TEST_CHECK(entity.get<TagComponent>() == &tag, "get and addComponent should return the same address");

        entity.remove(transform);

        TEST_CHECK(entity.get<Transform>() == nullptr,
            "getComponent should return nullptr for removed component reference");

        entity.remove<TagComponent>();

        TEST_CHECK(entity.get<TagComponent>() == nullptr, "getComponent should return nullptr for removed component type");
    }

    void EntitiesTest::testJsonSerialization()
    {
        DEBUG_LOG("\n= Starting json serialization tests =");

        const Scene toSerialize = makeScene();

        rapidjson::StringBuffer buffer;
        rapidjson::Writer       writer(buffer);
        TEST_CHECK(toSerialize.toJson(writer), "Scene json serialization failed");
        TEST_CHECK(writer.IsComplete(), "Scene json serialization failed - Produced json is incomplete");

        const std::string validJsonStr(buffer.GetString(), buffer.GetSize());

        const std::string invalidJsonStr = R"(
{
  "entities": 5
  "components": [
    {
      "type": "Transform",
      "data": {
        "position": "",
        "scale": ""
      }
    }
  ],
  "children": [yup...that's invalid]
})";

        rapidjson::Document validJson;
        validJson.Parse(validJsonStr.c_str(), validJsonStr.size());

        rapidjson::Document invalidJson;
        invalidJson.Parse(invalidJsonStr.c_str(), invalidJsonStr.size());

        Scene scene;
        TEST_CHECK(!scene.fromJson(invalidJson), "Scene deserialization from invalid json should have failed");
        TEST_CHECK(scene.fromJson(validJson), "Scene deserialization from valid json failed");

        {
            rapidjson::StringBuffer tmpBuffer;
            rapidjson::Writer       tmpWriter(tmpBuffer);
            TEST_CHECK(scene.toJson(tmpWriter), "Scene json serialization failed");
            TEST_CHECK(tmpWriter.IsComplete(), "Scene json serialization failed - Produced json is incomplete");

            const std::string tmpJsonStr(tmpBuffer.GetString(), tmpBuffer.GetSize());
            TEST_CHECK(tmpJsonStr == validJsonStr);
        }
    }

    void EntitiesTest::testBinarySerialization()
    {
        DEBUG_LOG("\n= Starting binary serialization tests =");

        const Scene toSerialize = makeScene();

        std::vector<char> validArray;
        TEST_CHECK(toSerialize.toBinary(validArray), "Scene binary serialization failed");

        std::vector<char> invalidArray = validArray;
        invalidArray.resize(invalidArray.size() / 2);

        Scene scene;
        TEST_CHECK(scene.fromBinary(invalidArray.data(), invalidArray.size()) == 0,
            "Scene deserialization from invalid byte array should have failed");

        TEST_CHECK(scene.fromBinary(validArray.data(), validArray.size()) != 0,
            "Scene deserialization from valid byte array failed");

        {
            std::vector<char> tmpArray;
            TEST_CHECK(scene.toBinary(tmpArray), "Scene binary serialization failed");
            TEST_CHECK(tmpArray == validArray);
        }
    }
}
