#include "PantheonTest/TestApplication.h"

#include "PantheonRendering/RHI/Null/NullShader.h"
#include "PantheonRendering/RHI/OpenGL/OpenGLShader.h"

#include "PantheonTest/ComponentRegistrations.h"
#include "PantheonTest/ResourceRegistrations.h"
#include "PantheonTest/Tests/AssetBundlesTest.h"
#include "PantheonTest/Tests/EntitiesTest.h"
#include "PantheonTest/Tests/InputTest.h"
#include "PantheonTest/Tests/ThreadPoolTest.h"
#include "PantheonTest/Tests/TypeTraitsTest.h"
#include "PantheonTest/Tests/WindowTest.h"

#include <PantheonCore/Debug/Logger.h>
#include <PantheonCore/Utility/FileSystem.h>
#include <PantheonCore/Utility/ServiceLocator.h>

#include <PantheonRendering/RHI/IRenderAPI.h>

#include <csignal>
#include <ctime>

using namespace LibMath;

using namespace PantheonCore::Utility;
using namespace PantheonCore::Resources;

using namespace PantheonApp::Core;
using namespace PantheonApp::Input;
using namespace PantheonApp::Windowing;

using namespace PantheonRendering::RHI;
using namespace PantheonRendering::Core;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::LowRenderer;
using namespace PantheonRendering::Resources;

namespace PantheonTest
{
    TestApplication::TestApplication()
#ifdef PTH_HEADLESS_TEST
        : IApplication(IContext::create(EGraphicsAPI::NONE, true, 4)),
#else
        : IApplication(IContext::create(EGraphicsAPI::OPENGL, true, 4)),
#endif
        m_window(std::make_unique<Window>(getContext(), WindowSettings{ "Pantheon Test", 800, 600 })),
        m_inputManager(std::make_unique<InputManager>(*m_window)),
        m_threadPool(std::make_unique<ThreadPool>()),
        m_resourceManager(std::make_unique<ResourceManager>()),
        m_renderer(std::make_unique<Renderer>()),
        m_startTime(std::chrono::high_resolution_clock::now())
    {
        ServiceLocator::provide<Window>(*m_window);
        ServiceLocator::provide<InputManager>(*m_inputManager);
        ServiceLocator::provide<ThreadPool>(*m_threadPool);
        ServiceLocator::provide<ResourceManager>(*m_resourceManager);

#ifndef PTH_HEADLESS_TEST
        m_tests.emplace_back(std::make_unique<WindowTest>());
#endif

        m_tests.emplace_back(std::make_unique<InputTest>());
        m_tests.emplace_back(std::make_unique<ThreadPoolTest>());
        m_tests.emplace_back(std::make_unique<EntitiesTest>());
        m_tests.emplace_back(std::make_unique<AssetBundlesTest>());
    }

    void TestApplication::onStart(int, char*[])
    {
        const char* appDir     = getApplicationDirectory();
        std::string workingDir = getWorkingDirectory();

        DEBUG_LOG("Changing working directory from \"%s\" to \"%s\"", workingDir.c_str(), appDir);
        changeDirectory(appDir);

        workingDir = getWorkingDirectory();
        DEBUG_LOG("Current working directory: \"%s\"", workingDir.c_str());
        ASSERT(workingDir == appDir, "Invalid working directory - Expected: \"%s\"", appDir);

        m_resourceManager->addSearchPath("assets");

        m_window->makeCurrentContext();

        IRenderAPI::getCurrent().init(true)
                                .setCullFace(ECullFace::BACK)
                                .setClearColor(Color::black)
                                .setCapability(ERenderingCapability::DEPTH_TEST, true)
                                .setViewport({ 0, 0 }, m_window->getSize());

        m_window->m_framebufferResizeEvent.subscribe([](const DimensionsT size)
        {
            IRenderAPI::getCurrent().setViewport({ 0, 0 }, size);
        });

        for (const auto& test : m_tests)
            test->start();

        // TODO: Move following code to its own test files
        {
            [[maybe_unused]] const ResourceRef tmp = m_resourceManager->load<IShader>("tmp", "shaders/Basic.glsl");
            ASSERT(tmp, "Failed to load shader");

            ASSERT(!m_resourceManager->load<ITexture>("tmp", "shaders/Basic.glsl"), "Invalid resource conversion");

            ASSERT(!m_resourceManager->load<ITexture>("tmp", "textures/grid.tga"), "Invalid resource conversion");
        }

        ASSERT(m_resourceManager->load<ITexture>("tmp", "textures/grid.tga"), "Valid resource conversion failed");

        [[maybe_unused]] const ResourceRef shader = m_resourceManager->load<IShader>("unlit", "shaders/Unlit.glsl");
        ASSERT(shader, "Failed to load shader");

#ifdef PTH_HEADLESS_TEST
        [[maybe_unused]] const ResourceRef castShader = m_resourceManager->load<NullShader>("unlit", "shaders/Unlit.glsl");
#else
        [[maybe_unused]] const ResourceRef castShader = m_resourceManager->load<OpenGLShader>("unlit", "shaders/Unlit.glsl");
#endif
        ASSERT(castShader, "Failed to reload resource with compatible type");
        ASSERT(castShader.getOrDefault() == shader.getOrDefault(),
            "Reloading resource with compatible type should preserve address");

        [[maybe_unused]] const ResourceRef model = m_resourceManager->load<Model>("cube", "meshes/primitives/cube.obj");
        ASSERT(model, "Failed to load model");

        [[maybe_unused]] const ResourceRef material = m_resourceManager->load<Material>("container", "materials/unlit.pthmat");
        ASSERT(material, "Failed to load material");
    }

    void TestApplication::preUpdate()
    {
        for (const auto& test : m_tests)
            test->preUpdate();
    }

    void TestApplication::update()
    {
        for (const auto& test : m_tests)
            test->update();

        IRenderAPI& renderAPI = IRenderAPI::getCurrent();
        renderAPI.clear(true, true, true);

        // static IShader*     shader   = m_resourceManager->get<IShader>("unlit");
        static const Model& model    = *m_resourceManager->get<Model>("cube");
        static Material*    material = m_resourceManager->get<Material>("container").getOrDefault();

        using namespace LibMath::Literal;
        const Matrix4 projMat   = perspectiveProjection(90_deg, m_window->getAspect(), .01f, 14.f);
        const Matrix4 viewMat   = lookAt({ 0.f, 1.8f, 1.f }, Vector3::zero(), Vector3::up());
        const Matrix4 modelMat1 = translation(-1.f, 0.f, 0.f) * rotation(45_deg, Vector3::up());
        const Matrix4 modelMat2 = translation(1.f, 0.f, 0.f) * rotation(-45_deg, Vector3::up());

        m_renderer->submit({
            &model.getMesh(0),
            material,
            modelMat1,
            model.getBoundingBox(),
            Layer::ALL
        });

        m_renderer->submit({
            &model.getMesh(0),
            material,
            modelMat2,
            model.getBoundingBox(),
            Layer::ALL
        });

        const Camera cam(projMat, viewMat, ECullingMode::MODEL);
        m_renderer->render(cam);
        m_renderer->clearQueue();
    }

    void TestApplication::postUpdate()
    {
        for (const auto& test : m_tests)
            test->postUpdate();

        m_window->swapBuffers();
        m_inputManager->update();
    }

    void TestApplication::onUpdate(const float)
    {
        preUpdate();
        update();
        postUpdate();
    }

    void TestApplication::onFixedUpdate(const float)
    {
        for (const auto& test : m_tests)
            test->fixedUpdate();
    }

    void TestApplication::onStop()
    {
        size_t passedCount = 0;
        for (const auto& test : m_tests)
        {
            test->stop();

            if (test->isSuccess())
                ++passedCount;
        }

        const auto      endTime     = std::chrono::high_resolution_clock::now();
        const long long elapsedTime = std::chrono::duration_cast<std::chrono::milliseconds>(endTime - m_startTime).count();

        if (passedCount == m_tests.size())
        {
            DEBUG_LOG("All %llu tests passed | Total execution time: %dms", passedCount, elapsedTime);
        }
        else
        {
            DEBUG_LOG_ERROR("%llu/%llu Tests passed | Total execution time: %dms", passedCount, m_tests.size(), elapsedTime);
            std::quick_exit(-1);
        }
    }

    bool TestApplication::isRunning() const
    {
#ifdef PTH_HEADLESS_TEST
        const auto isInProgress = [](const std::unique_ptr<ITest>& test)
        {
            return !test->isDone();
        };

        return !m_window->shouldClose() && std::ranges::find_if(m_tests, isInProgress) != m_tests.end();
#else
        return !m_window->shouldClose();
#endif
    }
}
