#pragma once
#include "PantheonRendering/LowRenderer/Camera.h"

#include <set>

namespace PantheonRendering::Resources
{
    class Mesh;
    class Material;
}

namespace PantheonRendering::Core
{
    class Renderer
    {
    public:
        static constexpr int CAMERA_UBO_INDEX = 0;
        static constexpr int MODEL_UBO_INDEX  = 1;

        struct DrawInfo
        {
            const Resources::Mesh*     m_mesh;
            const Resources::Material* m_material;
            LibMath::Matrix4           m_modelMat;
            Geometry::BoundingBox      m_modelBoundingBox;
            LayerMask                  m_layerMask;

            bool operator<(const DrawInfo& other) const;
        };

        virtual ~Renderer() = default;

        void submit(DrawInfo drawInfo);

        void render(const LowRenderer::Camera& camera) const;

        void clearQueue();

    protected:
        struct CameraUBO
        {
            LibMath::Matrix4 m_viewProjection;
            LibMath::Vector3 m_viewPos;
        };

        struct ModelUBO
        {
            LibMath::Matrix4x4 m_modelMat;
            LibMath::Matrix4x4 m_normalMat;
        };

        std::multiset<DrawInfo> m_renderQueue;
    };
}
