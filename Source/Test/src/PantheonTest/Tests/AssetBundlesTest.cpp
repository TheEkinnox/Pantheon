#include "PantheonTest/Tests/AssetBundlesTest.h"

#include <PantheonCore/Assets/AssetBundle.h>
#include <PantheonCore/Resources/ResourceAsset.h>
#include <PantheonCore/Utility/utility.h>

#include <filesystem>

using namespace PantheonCore::Assets;
using namespace PantheonCore::Utility;
using namespace PantheonCore::Resources;

namespace PantheonTest
{
    static void testAssetAtPath(const AssetBundle& bundle, const std::shared_ptr<const Asset>& asset);
    static void testAssetWithGuid(const AssetBundle& bundle, const std::shared_ptr<const Asset>& asset);

    AssetBundlesTest::AssetBundlesTest()
        : AssetBundlesTest("Asset bundles")
    {
    }

    AssetBundlesTest::AssetBundlesTest(const std::string& name)
        : ITest(name)
    {
    }

    void AssetBundlesTest::onStart()
    {
        testAdd();
        testSerialization();

        PTH_SERVICE(ResourceManager).clear();
        complete();
    }

    void AssetBundlesTest::testAdd()
    {
        {
            const ResourceAsset resourceAssets[] =
            {
                { "Texture", "c53a69fd-745c-4cfc-a5db-282947924bc2", "textures/container.jpg" },
                { "Material", "9ae1f054-4bfa-4aac-adb9-83af83278c0d", "materials/unlit.pthmat" },
                { "Material", "a1e05107-201f-4bda-b31b-e190f6e8e8e5", "materials/lit.pthmat" },
                { "Shader", "868b6811-6ab6-4d8a-becf-fabfd3f5d87c", "shaders/Lit.glsl" },
                { "Shader", "e8bfefe7-d3f6-43cb-a70d-ffd82c501c71", "shaders/Unlit.glsl" },
                { "Model", "0febc1d1-4569-4269-acdd-743557c4ecd6", "meshes/primitives/cube.obj" },
                { "Model", "59e50a16-ec45-4d7a-b068-89b64fb12535", "meshes/primitives/sphere.obj" },
                { "Scene", "8a5491ad-e3cf-4f32-a748-539170994753", "scenes/empty.pthscene" },
                { "Scene", "b6b3bba8-d4fe-4f14-9fac-a10a3bc87c0d", "scenes/pretty.pthscene" },
                { "Scene", "65e9a7e1-4986-4147-bf61-9b314e2e44db", "scenes/minified.pthscene" }
            };

            for (const auto& asset : resourceAssets)
            {
                TEST_CHECK(m_assetBundle.add(asset));
            }

            TEST_CHECK(!m_assetBundle.add(ResourceAsset("Mesh", resourceAssets[0].getGuid(), "UNUSED")),
                "Adding asset with existing guid should have failed");

            TEST_CHECK(!m_assetBundle.add(ResourceAsset("Mesh", "UNUSED", resourceAssets[0].getPath())),
                "Adding asset with existing path should have failed");
        }

        {
            Asset assets[] =
            {
                { "Text", "30507b5d-74b5-4514-a517-736d2197a6cc", "assets/shaders/Basic.glsl" },
                { "Binary", "c94e335e-5be6-4699-b9cc-4e537c75a812", "assets/textures/container2.png" }
            };

            for (const auto& asset : assets)
            {
                TEST_CHECK(m_assetBundle.add(asset), "Unable to add %s asset \"%s\" (path: \"%s\")",
                    asset.getType(), asset.getGuid(), asset.getPath());
            }
        }

        {
            TEST_CHECK(!m_assetBundle.add(Asset("unknown", "7cf05d0c-78ad-4597-bf47-0ded7c56dc48", "invalid.invalid")),
                "Adding invalid asset should have failed");
        }
    }

    void AssetBundlesTest::testSerialization()
    {
        static constexpr std::tuple<ECompressionMode, const char*, const char*> compressionModes[] =
        {
            { ECompressionMode::NONE, "none", "none.pthbundle" },
            { ECompressionMode::ZSTD, "zstd", "zstd.pthbundle" },
            { ECompressionMode::BROTLI, "brotli", "brotli.pthbundle" },
            { ECompressionMode::LZ4, "lz4", "lz4.pthbundle" }
        };

        double uncompressedSize = 0;

        for (const auto& [compressionMode, modeName, bundlePath] : compressionModes)
        {
            const auto start  = std::chrono::system_clock::now();
            const auto assets = m_assetBundle.getAssets();

            PTH_LOG("\nCompression mode: %s", modeName);

            TEST_CHECK(m_assetBundle.save(bundlePath, compressionMode),
                "Unbale to save asset bundle at path \"%s\"", bundlePath);

            const uint64_t bundleSize = std::filesystem::file_size(bundlePath);

            if (compressionMode == ECompressionMode::NONE)
                uncompressedSize = static_cast<double>(bundleSize);

            const auto   writeEnd  = std::chrono::system_clock::now();
            const double writeTime = std::chrono::duration_cast<std::chrono::duration<double>>(writeEnd - start).count();

            TEST_CHECK(m_assetBundle.load(bundlePath),
                "Unbale to load asset bundle at path \"%s\"", bundlePath);

            PTH_LOG("TEST PATHS");
            for (const auto& asset : assets)
                testAssetAtPath(m_assetBundle, asset);

            const auto   readEnd  = std::chrono::system_clock::now();
            const double readTime = std::chrono::duration_cast<std::chrono::duration<double>>(readEnd - writeEnd).count();

            PTH_LOG("TEST GUIDS");
            for (const auto& asset : assets)
                testAssetWithGuid(m_assetBundle, asset);

            std::filesystem::remove(bundlePath);

            const auto   end          = std::chrono::system_clock::now();
            const double testDuration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

            PTH_LOG("\n%s\t- Test completed in %fs\n"
                "\t- Write: %fs | Write avg.: %fs\n"
                "\t- Read: %fs | Read avg.: %fs\n"
                "\t- Bundle Size: %s | Compression Ratio: %f\n"
                "\t- Avg. Write Speed: %s/s | Avg. Read Speed: %s/s",
                modeName, testDuration,
                writeTime, writeTime / static_cast<double>(assets.size()),
                readTime, readTime / static_cast<double>(assets.size()),
                sizeToStr(static_cast<double>(bundleSize)).c_str(), uncompressedSize / static_cast<double>(bundleSize),
                sizeToStr(uncompressedSize / writeTime).c_str(), sizeToStr(uncompressedSize / readTime).c_str());
        }
    }

    static void testAssetAtPath(const AssetBundle& bundle, const std::shared_ptr<const Asset>& asset)
    {
        const std::vector<char> bundleData = bundle.getAssetAtPath(asset->getPath());

        std::vector<char> assetData;
        asset->getData(assetData);

        if (compareBuffers(assetData, bundleData))
            PTH_LOG("\"%s\": %s", asset->getPath(), "OK");
        else
            PTH_LOG_ERROR("\"%s\": %s", asset->getPath(), "INVALID");
    }

    static void testAssetWithGuid(const AssetBundle& bundle, const std::shared_ptr<const Asset>& asset)
    {
        const std::vector<char> bundleData = bundle.getAssetWithGuid(asset->getGuid());

        std::vector<char> assetData;
        asset->getData(assetData);

        if (compareBuffers(assetData, bundleData))
            PTH_LOG("\"%s\": %s", asset->getGuid(), "OK");
        else
            PTH_LOG_ERROR("\"%s\": %s", asset->getGuid(), "INVALID");
    }
}
