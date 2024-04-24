#include "PantheonRendering/Core/Camera.h"

using namespace LibMath;
using namespace PantheonRendering::Geometry;

namespace PantheonRendering::Core
{
    Camera::Camera()
        : Camera(Matrix4(1.f))
    {
    }

    Camera::Camera(Matrix4 viewProjection)
        : m_viewProjectionMatrix(std::move(viewProjection)), m_frustum(m_viewProjectionMatrix)
    {
    }

    const Matrix4& Camera::getViewProjection() const
    {
        return m_viewProjectionMatrix;
    }

    Camera& Camera::setViewProjection(const Matrix4& viewProjection)
    {
        m_viewProjectionMatrix = viewProjection;
        m_frustum              = Frustum(m_viewProjectionMatrix);

        return *this;
    }

    const Frustum& Camera::getFrustum() const
    {
        return m_frustum;
    }
}
