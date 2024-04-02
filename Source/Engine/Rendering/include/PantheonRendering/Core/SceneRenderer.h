#pragma once
#include "PantheonRendering/Core/Renderer.h"

#include <PantheonCore/ECS/Scene.h>

namespace PantheonRendering::Core
{
    class SceneRenderer : public Renderer
    {
        using CamerasList = std::vector<const LowRenderer::Camera*>;

    public:
        SceneRenderer()                               = default;
        SceneRenderer(const SceneRenderer& other)     = default;
        SceneRenderer(SceneRenderer&& other) noexcept = default;
        ~SceneRenderer() override                     = default;

        SceneRenderer& operator=(const SceneRenderer& other)     = default;
        SceneRenderer& operator=(SceneRenderer&& other) noexcept = default;

        void init(const PantheonCore::ECS::Scene& scene);

        void render(PantheonCore::ECS::Scene& scene, bool shouldInit = true);
    };
}
