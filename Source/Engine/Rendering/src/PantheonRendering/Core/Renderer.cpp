#include "PantheonRendering/Core/Renderer.h"

#include "PantheonRendering/Resources/Material.h"
#include "PantheonRendering/Resources/Mesh.h"
#include "PantheonRendering/RHI/IFrameBuffer.h"
#include "PantheonRendering/RHI/IRenderAPI.h"
#include "PantheonRendering/RHI/IUniformBuffer.h"

using namespace LibMath;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::Geometry;
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

    void Renderer::render(const RenderPass& renderPass)
    {
        ASSERT(renderPass.m_camera);

        if (renderPass.m_cullingMask == Layer::NONE)
            return;

        if (m_currentTarget != renderPass.m_target)
        {
            if (renderPass.m_target)
                renderPass.m_target->bind();
            else
                IFrameBuffer::bindDefault();

            m_currentTarget = renderPass.m_target;
        }

        IRenderAPI&   api    = IRenderAPI::getCurrent();
        const Camera& camera = *renderPass.m_camera;

        const Frustum&  frustum          = camera.getFrustum();
        const Material* previousMaterial = nullptr;
        const Mesh*     previousMesh     = nullptr;

        const std::unique_ptr<IUniformBuffer> camBuffer   = IUniformBuffer::create(EAccessMode::DYNAMIC_DRAW, CAMERA_UBO_INDEX);
        const std::unique_ptr<IUniformBuffer> modelBuffer = IUniformBuffer::create(EAccessMode::DYNAMIC_DRAW, MODEL_UBO_INDEX);

        CameraUBO camUBO
        {
            camera.getViewProjection().transposed(),
            renderPass.m_viewPos
        };

        camBuffer->setData(&camUBO, 1);
        camBuffer->bind();
        modelBuffer->bind();

        if (renderPass.m_shaderOverride)
            renderPass.m_shaderOverride->bind();

        for (const auto& drawInfo : m_renderQueue)
        {
            const auto& [mesh, material, modelMat, modelBoundingBox, layerMask, extra] = drawInfo;

            if ((renderPass.m_cullingMask & layerMask) == 0)
                continue;

            switch (renderPass.m_cullingMode)
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

            const bool materialChanged = (!renderPass.m_shaderOverride && previousMaterial != material);

            if (materialChanged || previousMesh != mesh)
            {
                mesh->bind();
                previousMesh = mesh;
            }

            if (materialChanged)
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

            if (renderPass.m_onDraw)
                renderPass.m_onDraw(drawInfo);

            api.drawElements(mesh->getPrimitiveType(), mesh->getIndexCount());
        }

        camBuffer->unbind();
        modelBuffer->unbind();

        if (previousMaterial)
            previousMaterial->getShader().unbind();

        if (renderPass.m_shaderOverride)
            renderPass.m_shaderOverride->unbind();

        if (previousMesh)
            previousMesh->unbind();

        if (renderPass.m_target)
            renderPass.m_target->unbind();
    }

    void Renderer::clearQueue()
    {
        m_renderQueue.clear();
    }
}
