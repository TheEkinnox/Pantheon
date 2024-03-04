#include "PantheonTest/Tests/EntitiesTest.h"

#include "PantheonCore/ECS/Components/TagComponent.h"
#include "PantheonCore/ECS/Components/TransformComponent.h"

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

    void EntitiesTest::onStart()
    {
        m_entities.clear();

        testComponents();
        testJsonSerialization();
        testBinarySerialization();

        complete();
    }

    void EntitiesTest::testComponents()
    {
        Entity& entity = m_entities.emplace_back();

        TEST_CHECK(entity.getComponents<Component>().empty(), "Components list of a new entity should be empty");

        const TransformComponent* transform = entity.addComponent<TransformComponent>();
        const TagComponent*       tag       = entity.addComponent<TagComponent>("Test");

        TEST_CHECK(entity.getComponent<TransformComponent>(transform->getId()) == transform,
            "get and addComponent should return the same address");
        TEST_CHECK(entity.getComponent<TransformComponent>() == transform, "get and addComponent should return the same address");

        TEST_CHECK(entity.getComponent<TagComponent>(tag->getId()) == tag, "get and addComponent should return the same address");
        TEST_CHECK(entity.getComponent<TagComponent>() == tag, "get and addComponent should return the same address");

        size_t componentsCount = entity.getComponents<Component>().size();
        TEST_CHECK(componentsCount == 2, "Components count should be 2. Received: %d", componentsCount);

        TEST_CHECK(entity.addComponent<TransformComponent>() == nullptr,
            "Adding a transform to an entity with an existing transform should have failed");

        size_t tmpCount = entity.getComponents<Component>().size();
        TEST_CHECK(tmpCount == componentsCount, "Components count should be %d. Received: %d", componentsCount, tmpCount);

        TEST_CHECK(entity.addComponent<TagComponent>() == nullptr,
            "Adding a tag to an entity with an existing tag should have failed");

        tmpCount = entity.getComponents<Component>().size();
        TEST_CHECK(tmpCount == componentsCount, "Components count should be %d. Received: %d", componentsCount, tmpCount);

        entity.removeComponent(*transform);

        TEST_CHECK(entity.getComponent<TransformComponent>() == nullptr,
            "getComponent should return nullptr for removed component reference");

        componentsCount = entity.getComponents<Component>().size();
        TEST_CHECK(componentsCount == 1, "Components count should be 1. Received: %d", componentsCount);

        entity.removeComponent<TagComponent>();

        TEST_CHECK(entity.getComponent<TagComponent>() == nullptr, "getComponent should return nullptr for removed component type");

        componentsCount = entity.getComponents<Component>().size();
        TEST_CHECK(componentsCount == 0, "Components count should be 0. Received: %d", componentsCount);
    }

    void EntitiesTest::testJsonSerialization()
    {
        DEBUG_LOG("\n= Starting json serialization tests =");

        Entity toSerialize;

        const Vector3             position  = Vector3(10, 20, 30), scale = Vector3(.7f, 8.f, 9.f);
        const Quaternion          rotation  = Quaternion(60_deg, -35_deg, 20_deg);
        const TransformComponent* transform = toSerialize.addComponent<TransformComponent>(Transform(position, rotation, scale));
        const Matrix4             matrix    = transform->getWorldMatrix();

        rapidjson::StringBuffer buffer;
        rapidjson::Writer       writer(buffer);
        TEST_CHECK(toSerialize.serialize(writer), "Entity json serialization failed");
        // TEST_CHECK(writer.IsComplete(), "Entity json serialization failed - Produced json is incomplete");

        Entity            entity;
        const std::string validJsonStr(buffer.GetString(), buffer.GetSize());

        const std::string invalidJsonStr = R"(
{
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

        TEST_CHECK(!entity.deserialize(invalidJson), "Entity deserialization from invalid json should have failed");

        TEST_CHECK(entity.deserialize(validJson), "Entity deserialization from valid json failed");

        transform = entity.getComponent<TransformComponent>();
        TEST_CHECK(transform != nullptr, "Missing transform component");

        if (transform)
        {
            TEST_CHECK(transform->getPosition() == position, "Invalid entity position %s - Expected %s",
                transform->getPosition().string().c_str(), position.string().c_str());

            TEST_CHECK(transform->getRotation() == rotation, "Invalid entity rotation %s - Expected %s",
                transform->getRotation().string().c_str(), rotation.string().c_str());

            TEST_CHECK(transform->getScale() == scale, "Invalid entity scale %s - Expected %s",
                transform->getScale().string().c_str(), scale.string().c_str());

            TEST_CHECK(transform->getWorldPosition() == position, "Invalid entity world position %s - Expected %s",
                transform->getWorldPosition().string().c_str(), position.string().c_str());

            TEST_CHECK(transform->getWorldRotation() == rotation, "Invalid entity world rotation %s - Expected %s",
                transform->getWorldRotation().string().c_str(), rotation.string().c_str());

            TEST_CHECK(transform->getWorldScale() == scale, "Invalid entity world scale %s - Expected %s",
                transform->getWorldScale().string().c_str(), scale.string().c_str());

            TEST_CHECK(transform->getMatrix() == matrix, "Invalid entity transformation matrix %s - Expected %s",
                transform->getMatrix().stringLong().c_str(), matrix.stringLong().c_str());

            TEST_CHECK(transform->getWorldMatrix() == matrix, "Invalid entity transformation matrix %s - Expected %s",
                transform->getMatrix().stringLong().c_str(), matrix.stringLong().c_str());

            TEST_CHECK(transform->getMatrix() == matrix, "Invalid entity transformation matrix");

            TEST_CHECK(transform->getWorldMatrix() == matrix, "Invalid entity world transformation matrix");
        }

        m_entities.push_back(entity);
    }

    void EntitiesTest::testBinarySerialization()
    {
        DEBUG_LOG("\n= Starting binary serialization tests =");

        Entity toSerialize;

        const Vector3             position  = Vector3(126.f, -31.43f, 252.525f), scale = Vector3(.7f, 8.f, 9.f);
        const Quaternion          rotation  = Quaternion(32.5_deg, 49.3_deg, -10.9_deg);
        const TransformComponent* transform = toSerialize.addComponent<TransformComponent>(Transform(position, rotation, scale));
        const Matrix4             matrix    = transform->getWorldMatrix();

        std::vector<char> validArray;
        TEST_CHECK(toSerialize.serialize(validArray), "Entity binary serialization failed");

        std::vector<char> invalidArray = validArray;
        invalidArray.resize(invalidArray.size() / 2);

        Entity entity;

        TEST_CHECK(entity.deserialize(invalidArray.data(), invalidArray.size()) == 0,
            "Entity deserialization from invalid byte array should have failed");

        TEST_CHECK(entity.deserialize(validArray.data(), validArray.size()) != 0);

        transform = entity.getComponent<TransformComponent>();
        TEST_CHECK(transform != nullptr, "Missing transform component");

        if (transform)
        {
            TEST_CHECK(transform->getPosition() == position, "Invalid entity position %s - Expected %s",
                transform->getPosition().string().c_str(), position.string().c_str());

            TEST_CHECK(transform->getRotation() == rotation, "Invalid entity rotation %s - Expected %s",
                transform->getRotation().string().c_str(), rotation.string().c_str());

            TEST_CHECK(transform->getScale() == scale, "Invalid entity scale %s - Expected %s",
                transform->getScale().string().c_str(), scale.string().c_str());

            TEST_CHECK(transform->getWorldPosition() == position, "Invalid entity world position %s - Expected %s",
                transform->getWorldPosition().string().c_str(), position.string().c_str());

            TEST_CHECK(transform->getWorldRotation() == rotation, "Invalid entity world rotation %s - Expected %s",
                transform->getWorldRotation().string().c_str(), rotation.string().c_str());

            TEST_CHECK(transform->getWorldScale() == scale, "Invalid entity world scale %s - Expected %s",
                transform->getWorldScale().string().c_str(), scale.string().c_str());

            TEST_CHECK(transform->getMatrix() == matrix, "Invalid entity transformation matrix %s - Expected %s",
                transform->getMatrix().stringLong().c_str(), matrix.stringLong().c_str());

            TEST_CHECK(transform->getWorldMatrix() == matrix, "Invalid entity transformation matrix %s - Expected %s",
                transform->getMatrix().stringLong().c_str(), matrix.stringLong().c_str());

            TEST_CHECK(transform->getMatrix() == matrix, "Invalid entity transformation matrix");

            TEST_CHECK(transform->getWorldMatrix() == matrix, "Invalid entity world transformation matrix");
        }

        m_entities.push_back(entity);
    }
}
