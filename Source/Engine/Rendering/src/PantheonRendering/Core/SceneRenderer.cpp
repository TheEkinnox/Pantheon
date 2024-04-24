#include "PantheonRendering/Core/SceneRenderer.h"

#include "PantheonRendering/Components/CameraComponent.h"
#include "PantheonRendering/Components/ModelComponent.h"
#include "PantheonRendering/RHI/IRenderAPI.h"

#include <PantheonCore/ECS/SceneView.h>

#include <Transform.h>

using namespace LibMath;
using namespace PantheonCore::ECS;
using namespace PantheonRendering::RHI;
using namespace PantheonRendering::Resources;
using namespace PantheonRendering::Components;

namespace PantheonRendering::Core
{
    void SceneRenderer::init(const Scene& scene)
    {
        clearQueue();

        SceneView<const ModelComponent, const Transform> models(scene);

        for (const auto entity : models)
        {
            auto [modelComponent, transform] = models.get(entity);

            const Model*                model       = **modelComponent;
            const Matrix4               modelMatrix = transform ? transform->getWorldMatrix() : Matrix4(1.f);
            const Geometry::BoundingBox boundingBox = model->getBoundingBox();

            for (size_t i = 0; i < model->getMeshCount(); ++i)
            {
                submit({
                    &model->getMesh(i),
                    &modelComponent->getMaterial(model->getMaterialIndex(i)),
                    modelMatrix,
                    boundingBox,
                    modelComponent->getLayerMask()
                });
            }
        }
    }

    void SceneRenderer::render(Scene& scene, const bool shouldInit)
    {
        if (shouldInit)
            init(scene);

        IRenderAPI& api = IRenderAPI::getCurrent();

        const Color                initialClearColor = api.getClearColor();
        SceneView<CameraComponent> cameras(scene);

        for (const auto entity : cameras)
        {
            CameraComponent& cam = *cameras.get<CameraComponent>(entity);

            const PantheonCore::Resources::ResourceRef<RenderTarget> target = cam.getTarget();

            RenderPass renderPass{
                .m_camera = &*cam,
                .m_target = target ? &target->getFrameBuffer() : nullptr,
                .m_viewPos = Vector3::zero(),
                .m_cullingMask = cam.getCullingMask(),
                .m_cullingMode = cam.getCullingMode()
            };

            if (const Transform* transform = scene.get<const Transform>(entity))
            {
                cam.recalculate(transform->getWorldMatrix().inverse());
                renderPass.m_viewPos = transform->getWorldPosition();
            }
            else
            {
                cam.recalculate(Matrix4(1.f));
            }

            api.setClearColor(cam.getClearColor());

            bool clearColor, clearDepth, clearStencil;
            cam.getClearMask(clearColor, clearDepth, clearStencil);
            api.clear(clearColor, clearDepth, clearStencil);

            Renderer::render(renderPass);
        }

        clearQueue();
        api.setClearColor(initialClearColor);
    }
}
