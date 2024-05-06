#pragma once
#include "PantheonRendering/Core/Camera.h"
#include "PantheonRendering/Core/Layer.h"
#include "PantheonRendering/Enums/ECullingMode.h"

#include <set>

namespace PantheonRendering::RHI
{
    class IShader;
    class IFrameBuffer;
}

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
            LibMath::BoundingBox       m_modelBoundingBox;
            LayerMask                  m_layerMask;
            void*                      m_extra = nullptr;

            bool operator<(const DrawInfo& other) const;
        };

        struct RenderPass
        {
            const Camera*       m_camera;
            RHI::IFrameBuffer*  m_target;
            LibMath::Vector3    m_viewPos;
            LayerMask           m_cullingMask;
            Enums::ECullingMode m_cullingMode;

            RHI::IShader* m_shaderOverride                   = nullptr;
            void (*       m_onDraw)(const DrawInfo& payload) = nullptr;
        };

        virtual ~Renderer() = default;

        void submit(DrawInfo drawInfo);

        void render(const RenderPass& camera);

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

    private:
        RHI::IFrameBuffer*      m_currentTarget = nullptr;
        std::multiset<DrawInfo> m_renderQueue;
    };
}
