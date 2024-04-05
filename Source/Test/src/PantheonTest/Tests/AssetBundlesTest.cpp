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

        complete();
    }

    void AssetBundlesTest::testAdd()
    {
        {
            std::vector<ResourceAsset> resourceAssets;

            resourceAssets.emplace_back("Texture", "c53a69fd-745c-4cfc-a5db-282947924bc2", "textures/container.jpg");
            resourceAssets.emplace_back("Material", "9ae1f054-4bfa-4aac-adb9-83af83278c0d", "materials/unlit.pthmat");
            resourceAssets.emplace_back("Material", "a1e05107-201f-4bda-b31b-e190f6e8e8e5", "materials/lit.pthmat");
            resourceAssets.emplace_back("Shader", "868b6811-6ab6-4d8a-becf-fabfd3f5d87c", "shaders/Lit.glsl");
            resourceAssets.emplace_back("Shader", "e8bfefe7-d3f6-43cb-a70d-ffd82c501c71", "shaders/Unlit.glsl");
            resourceAssets.emplace_back("Model", "0febc1d1-4569-4269-acdd-743557c4ecd6", "meshes/primitives/cube.obj");
            resourceAssets.emplace_back("Model", "59e50a16-ec45-4d7a-b068-89b64fb12535", "meshes/primitives/sphere.obj");
            resourceAssets.emplace_back("Scene", "8a5491ad-e3cf-4f32-a748-539170994753", "scenes/empty.pthscene");
            resourceAssets.emplace_back("Scene", "b6b3bba8-d4fe-4f14-9fac-a10a3bc87c0d", "scenes/pretty.pthscene");
            resourceAssets.emplace_back("Scene", "65e9a7e1-4986-4147-bf61-9b314e2e44db", "scenes/minified.pthscene");

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
            std::vector<Asset> assets;

            assets.emplace_back("Text", "30507b5d-74b5-4514-a517-736d2197a6cc", "assets/shaders/Basic.glsl");
            assets.emplace_back("Binary", "c94e335e-5be6-4699-b9cc-4e537c75a812", "assets/textures/container2.png");

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
        std::vector<std::pair<ECompressionMode, std::string>> compressionModes;

        compressionModes.emplace_back(ECompressionMode::NONE, "none");
        compressionModes.emplace_back(ECompressionMode::ZSTD, "zstd");
        compressionModes.emplace_back(ECompressionMode::BROTLI, "brotli");
        compressionModes.emplace_back(ECompressionMode::LZ4, "lz4");

        double uncompressedSize = 0;

        for (const auto& [compressionMode, modeName] : compressionModes)
        {
            auto       start  = std::chrono::system_clock::now();
            const auto assets = m_assetBundle.getAssets();

            std::string bundlePath = modeName + ".pthbundle";
            DEBUG_LOG("\nCompression mode: %s", modeName.c_str());

            TEST_CHECK(m_assetBundle.save(bundlePath.c_str(), compressionMode),
                "Unbale to save asset bundle at path \"%s\"", bundlePath.c_str());

            const uint64_t bundleSize = std::filesystem::file_size(bundlePath);

            if (compressionMode == ECompressionMode::NONE)
                uncompressedSize = static_cast<double>(bundleSize);

            auto         writeEnd  = std::chrono::system_clock::now();
            const double writeTime = std::chrono::duration_cast<std::chrono::duration<double>>(writeEnd - start).count();

            TEST_CHECK(m_assetBundle.load(bundlePath),
                "Unbale to load asset bundle at path \"%s\"", bundlePath.c_str());

            DEBUG_LOG("TEST PATHS");
            for (const auto& asset : assets)
                testAssetAtPath(m_assetBundle, asset);

            auto         readEnd  = std::chrono::system_clock::now();
            const double readTime = std::chrono::duration_cast<std::chrono::duration<double>>(readEnd - writeEnd).count();

            DEBUG_LOG("TEST GUIDS");
            for (const auto& asset : assets)
                testAssetWithGuid(m_assetBundle, asset);

            std::filesystem::remove(bundlePath);

            auto         end          = std::chrono::system_clock::now();
            const double testDuration = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

            DEBUG_LOG("\n%s\t- Test completed in %fs\n"
                "\t- Write: %fs | Write avg.: %fs\n"
                "\t- Read: %fs | Read avg.: %fs\n"
                "\t- Bundle Size: %s | Compression Ratio: %f\n"
                "\t- Avg. Write Speed: %s/s | Avg. Read Speed: %s/s",
                modeName.c_str(), testDuration,
                writeTime, writeTime / static_cast<double>(assets.size()),
                readTime, readTime / static_cast<double>(assets.size()),
                sizeToStr(static_cast<double>(bundleSize)).c_str(), uncompressedSize / static_cast<double>(bundleSize),
                sizeToStr(uncompressedSize / writeTime).c_str(), sizeToStr(uncompressedSize / readTime).c_str());
        }
    }

    void AssetBundlesTest::testAssetAtPath(const AssetBundle& bundle, const std::shared_ptr<const Asset>& asset)
    {
        const std::vector<char> bundleData = bundle.getAssetAtPath(asset->getPath());

        std::vector<char> assetData;
        asset->getData(assetData);

        if (compareBuffers(assetData, bundleData))
            DEBUG_LOG("\"%s\": %s", asset->getPath(), "OK");
        else
            DEBUG_LOG_ERROR("\"%s\": %s", asset->getPath(), "INVALID");
    }

    void AssetBundlesTest::testAssetWithGuid(const AssetBundle& bundle, const std::shared_ptr<const Asset>& asset)
    {
        const std::vector<char> bundleData = bundle.getAssetWithGuid(asset->getGuid());

        std::vector<char> assetData;
        asset->getData(assetData);

        if (compareBuffers(assetData, bundleData))
            DEBUG_LOG("\"%s\": %s", asset->getGuid(), "OK");
        else
            DEBUG_LOG_ERROR("\"%s\": %s", asset->getGuid(), "INVALID");
    }

    bool AssetBundlesTest::compareBuffers(const std::vector<char>& buffer1, const std::vector<char>& buffer2)
    {
        if (buffer1.empty() && !buffer2.empty() || buffer2.empty() && !buffer1.empty())
            return false;

        if (buffer1.data() == buffer2.data())
            return true;

        if (buffer1.size() != buffer2.size())
            return false;

        return memcmp(buffer1.data(), buffer2.data(), buffer1.size()) == 0;
    }
}
