#pragma once
#include "ITest.h"

#include <PantheonCore/ECS/Scene.h>

namespace PantheonTest
{
    class EntitiesTest final : public ITest
    {
    public:
        EntitiesTest();
        explicit EntitiesTest(const std::string& name);

        void onStart() override;

    private:
        void testEntityStorage();
        void testComponentStorage();
        void testComponents();
        void testScene();

        static PantheonCore::ECS::Scene makeScene();

        void testJsonSerialization();
        void testBinarySerialization();
    };
}
