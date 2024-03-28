#include "PantheonRendering/LowRenderer/Camera.h"

using namespace LibMath;
using namespace PantheonRendering::Core;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::Geometry;

namespace PantheonRendering::LowRenderer
{
    Camera::Camera()
        : Camera({}, {}, ECullingMode::NONE)
    {
    }

    Camera::Camera(Matrix4 projection, Matrix4 view, const ECullingMode cullingMode)
        : m_viewMatrix(std::move(view)), m_projectionMatrix(std::move(projection)),
        m_viewProjectionMatrix(m_projectionMatrix * m_viewMatrix), m_frustum(m_viewProjectionMatrix), m_cullingMode(cullingMode)
    {
    }

    const Matrix4& Camera::getView() const
    {
        return m_viewMatrix;
    }

    Camera& Camera::setView(const Matrix4& view)
    {
        m_viewMatrix = view;
        onChange();

        return *this;
    }

    const Matrix4& Camera::getProjection() const
    {
        return m_projectionMatrix;
    }

    Camera& Camera::setProjection(const Matrix4& projection)
    {
        m_projectionMatrix = projection;
        onChange();

        return *this;
    }

    const Matrix4& Camera::getViewProjection() const
    {
        return m_viewProjectionMatrix;
    }

    const Frustum& Camera::getFrustum() const
    {
        return m_frustum;
    }

    ECullingMode Camera::getCullingMode() const
    {
        return m_cullingMode;
    }

    Camera& Camera::setCullingMode(ECullingMode cullingMode)
    {
        m_cullingMode = cullingMode;
        return *this;
    }

    Camera& Camera::setClearColor(const Color& color)
    {
        m_clearColor = color;
        return *this;
    }

    Camera& Camera::setClearColor(const float r, const float g, const float b, const float a)
    {
        setClearColor({ r, g, b, a });
        return *this;
    }

    Color Camera::getClearColor() const
    {
        return m_clearColor;
    }

    uint8_t Camera::getClearMask() const
    {
        return m_clearMask;
    }

    void Camera::setClearMask(const uint8_t clearMask)
    {
        m_clearMask = clearMask;
    }

    void Camera::getClearMask(bool& clearColor, bool& clearDepth, bool& clearStencil) const
    {
        clearColor   = m_clearMask & CLEAR_COLOR_BIT;
        clearDepth   = m_clearMask & CLEAR_DEPTH_BIT;
        clearStencil = m_clearMask & CLEAR_STENCIL_BIT;
    }

    void Camera::setClearMask(bool clearColor, bool clearDepth, bool clearStencil)
    {
        m_clearMask = static_cast<uint8_t>(
            clearColor << CLEAR_COLOR_OFFSET |
            clearDepth << CLEAR_DEPTH_OFFSET |
            clearStencil << CLEAR_STENCIL_OFFSET
        );
    }

    LayerMask Camera::getCullingMask() const
    {
        return m_cullingMask;
    }

    void Camera::setCullingMask(const LayerMask cullingMask)
    {
        m_cullingMask = cullingMask;
    }

    void Camera::onChange()
    {
        m_viewProjectionMatrix = m_projectionMatrix * m_viewMatrix;
        m_frustum              = Frustum(m_viewProjectionMatrix);
    }
}
