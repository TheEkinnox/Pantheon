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

        void onStart() override;

    private:
        PantheonCore::Assets::AssetBundle m_assetBundle;

        void testAdd();
        void testSerialization();

        static void testAssetAtPath(
            const PantheonCore::Assets::AssetBundle& bundle, const std::shared_ptr<const PantheonCore::Assets::Asset>& asset);

        static void testAssetWithGuid(
            const PantheonCore::Assets::AssetBundle& bundle, const std::shared_ptr<const PantheonCore::Assets::Asset>& asset);
    };
}
