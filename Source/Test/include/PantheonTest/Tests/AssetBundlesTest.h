#pragma once
#include "ITest.h"

#include <PantheonCore/Assets/AssetBundle.h>

namespace PantheonTest
{
    class AssetBundlesTest final : public ITest
    {
    public:
        AssetBundlesTest();
        explicit AssetBundlesTest(const std::string& name);

    protected:
        void onStart() override;

    private:
        PantheonCore::Assets::AssetBundle m_assetBundle;

        void testAdd();
        void testSerialization();
    };
}
