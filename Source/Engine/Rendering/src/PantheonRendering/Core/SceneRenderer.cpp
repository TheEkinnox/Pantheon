#include "PantheonRendering/Core/SceneRenderer.h"

#include "PantheonRendering/Components/ModelComponent.h"
#include "PantheonRendering/RHI/IRenderAPI.h"

#include <PantheonCore/ECS/SceneView.h>

#include <Transform.h>

using namespace LibMath;
using namespace PantheonCore::ECS;
using namespace PantheonRendering::RHI;
using namespace PantheonRendering::Resources;
using namespace PantheonRendering::Components;
using namespace PantheonRendering::LowRenderer;

namespace PantheonRendering::Core
{
    void SceneRenderer::init(const Scene& scene)
    {
        m_renderQueue.clear();

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

        const Color       initialClearColor = api.getClearColor();
        SceneView<Camera> cameras(scene);

        for (const auto entity : cameras)
        {
            Camera& cam = *cameras.get<Camera>(entity);

            if (const Transform* transform = scene.get<const Transform>(entity))
                cam.setView(transform->getWorldMatrix().inverse());

            api.setClearColor(cam.getClearColor());

            bool clearColor, clearDepth, clearStencil;
            cam.getClearMask(clearColor, clearDepth, clearStencil);
            api.clear(clearColor, clearDepth, clearStencil);

            Renderer::render(cam);
        }

        clearQueue();
        api.setClearColor(initialClearColor);
    }
}
