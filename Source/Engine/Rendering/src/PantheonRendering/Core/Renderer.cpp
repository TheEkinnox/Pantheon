#include "PantheonRendering/Core/Renderer.h"

#include "PantheonRendering/Resources/Material.h"
#include "PantheonRendering/Resources/Mesh.h"
#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/IUniformBuffer.h"

using namespace LibMath;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::Geometry;
using namespace PantheonRendering::LowRenderer;
using namespace PantheonRendering::Resources;
using namespace PantheonRendering::RHI;

namespace PantheonRendering::Core
{
    bool Renderer::DrawInfo::operator<(const DrawInfo& other) const
    {
        return m_material < other.m_material || m_mesh < other.m_mesh;
    }

    void Renderer::submit(DrawInfo drawInfo)
    {
        ASSERT(drawInfo.m_material != nullptr && drawInfo.m_mesh != nullptr);

        if (drawInfo.m_layerMask != Layer::NONE)
            m_renderQueue.emplace(std::move(drawInfo));
    }

    void Renderer::render(const Camera& camera) const
    {
        const LayerMask camLayerMask = camera.getCullingMask();

        if (camLayerMask == Layer::NONE)
            return;

        IRenderAPI& api = IRenderAPI::getCurrent();

        const Frustum&  frustum          = camera.getFrustum();
        const Material* previousMaterial = nullptr;
        const Mesh*     previousMesh     = nullptr;

        const std::unique_ptr<IUniformBuffer> camBuffer   = IUniformBuffer::create(EAccessMode::DYNAMIC_DRAW, CAMERA_UBO_INDEX);
        const std::unique_ptr<IUniformBuffer> modelBuffer = IUniformBuffer::create(EAccessMode::DYNAMIC_DRAW, MODEL_UBO_INDEX);

        CameraUBO camUBO
        {
            camera.getViewProjection().transposed(),
            (camera.getView().inverse() * Vector4(.0f, .0f, .0f, 1.f)).xyz()
        };

        camBuffer->setData(&camUBO, 1);
        camBuffer->bind();
        modelBuffer->bind();

        for (const auto& [mesh, material, modelMat, modelBoundingBox, layerMask] : m_renderQueue)
        {
            if ((camLayerMask & layerMask) == 0)
                continue;

            switch (camera.getCullingMode())
            {
            case ECullingMode::MODEL:
            {
                if (!frustum.intersects(transformBoundingBox(modelBoundingBox, modelMat)))
                    continue;

                break;
            }
            case ECullingMode::MESH:
            {
                if (!frustum.intersects(transformBoundingBox(mesh->getBoundingBox(), modelMat)))
                    continue;

                break;
            }
            case ECullingMode::NONE:
            default:
                break;
            }

            if (previousMaterial != material || previousMesh != mesh)
            {
                mesh->bind();
                previousMesh = mesh;
            }

            if (previousMaterial != material)
            {
                material->bind();
                previousMaterial = material;
            }

            ModelUBO modelUbo
            {
                modelMat.transposed(),
                modelMat.inverse()
            };

            modelBuffer->setData(&modelUbo, 1);

            api.drawElements(mesh->getPrimitiveType(), mesh->getIndexCount());
        }

        camBuffer->unbind();
        modelBuffer->unbind();
        previousMaterial->getShader().unbind();
        previousMesh->unbind();
    }

    void Renderer::clearQueue()
    {
        m_renderQueue.clear();
    }
}
