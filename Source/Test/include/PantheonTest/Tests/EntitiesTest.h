#pragma once
#include "ITest.h"

#include <PantheonCore/ECS/Entity.h>

namespace PantheonTest
{
    class EntitiesTest final : public ITest
    {
    public:
        EntitiesTest();
        explicit EntitiesTest(const std::string& name);

        void onStart() override;

    private:
        std::vector<PantheonCore::ECS::Entity> m_entities;

        void testComponents();
        void testJsonSerialization();
        void testBinarySerialization();
    };
}
