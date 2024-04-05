#include "PantheonRendering/Components/CameraComponent.h"

using namespace LibMath;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::LowRenderer;

namespace PantheonRendering::Components
{
    CameraComponent::operator Camera() const
    {
        return m_camera;
    }

    void CameraComponent::recalculate()
    {
        if (m_projectionType == EProjectionType::PERSPECTIVE)
        {
            m_camera.setProjection(perspectiveProjection(m_fovY, m_aspect, m_perspectiveNear, m_perspectiveFar));
            return;
        }

        const float halfHeight = m_orthographicSize * .5f;
        const float halfWidth  = halfHeight * m_aspect;

        m_camera.setProjection(orthographicProjection(-halfWidth, halfWidth, -halfHeight, halfHeight,
            m_orthographicNear, m_orthographicFar));
    }

    EProjectionType CameraComponent::getProjectionType() const
    {
        return m_projectionType;
    }

    void CameraComponent::setProjectionType(const EProjectionType projectionType)
    {
        m_projectionType = projectionType;
        recalculate();
    }

    float CameraComponent::getAspect() const
    {
        return m_aspect;
    }

    void CameraComponent::setAspect(const float aspect)
    {
        m_aspect = aspect;
        recalculate();
    }

    Radian CameraComponent::getFovY() const
    {
        return m_fovY;
    }

    void CameraComponent::setFovY(const Radian fovY)
    {
        m_fovY = fovY;
        recalculate();
    }

    float CameraComponent::getPerspectiveNear() const
    {
        return m_perspectiveNear;
    }

    void CameraComponent::setPerspectiveNear(const float zNear)
    {
        m_perspectiveNear = zNear;
        recalculate();
    }

    float CameraComponent::getPerspectiveFar() const
    {
        return m_perspectiveFar;
    }

    void CameraComponent::setPerspectiveFar(const float zFar)
    {
        m_perspectiveFar = zFar;
        recalculate();
    }

    void CameraComponent::setPerspective(Radian fovY, const float zNear, const float zFar)
    {
        m_projectionType  = EProjectionType::PERSPECTIVE;
        m_perspectiveNear = zNear;
        m_perspectiveFar  = zFar;
        m_fovY            = fovY;

        recalculate();
    }

    float CameraComponent::getOrthographicSize() const
    {
        return m_orthographicSize;
    }

    void CameraComponent::setOrthographicSize(const float size)
    {
        m_orthographicSize = size;
        recalculate();
    }

    float CameraComponent::getOrthographicNear() const
    {
        return m_orthographicNear;
    }

    void CameraComponent::setOrthographicNear(const float zNear)
    {
        m_orthographicNear = zNear;
        recalculate();
    }

    float CameraComponent::getOrthographicFar() const
    {
        return m_orthographicFar;
    }

    void CameraComponent::setOrthographicFar(const float zFar)
    {
        m_orthographicFar = zFar;
        recalculate();
    }

    void CameraComponent::setOrthographic(const float size, const float zNear, const float zFar)
    {
        m_projectionType   = EProjectionType::ORTHOGRAPHIC;
        m_orthographicSize = size;
        m_orthographicNear = zNear;
        m_orthographicFar  = zFar;

        recalculate();
    }
}

namespace PantheonCore::ECS
{
    using namespace PantheonRendering::Components;
    using namespace Serialization;

    template <>
    bool ComponentRegistry::toBinary(const CameraComponent& component, std::vector<char>& out, const EntitiesMap&)
    {
        if (!CHECK(IByteSerializable::serializeString(projectionTypeToString(component.getProjectionType()), out)))
            return false;

        if (!CHECK(IByteSerializable::writeNumber(component.getFovY().raw(), out)))
            return false;

        if (!CHECK(IByteSerializable::writeNumber(component.getPerspectiveNear(), out)))
            return false;

        if (!CHECK(IByteSerializable::writeNumber(component.getPerspectiveFar(), out)))
            return false;

        if (!CHECK(IByteSerializable::writeNumber(component.getOrthographicSize(), out)))
            return false;

        if (!CHECK(IByteSerializable::writeNumber(component.getOrthographicNear(), out)))
            return false;

        return CHECK(IByteSerializable::writeNumber(component.getOrthographicFar(), out));
    }

    template <>
    size_t ComponentRegistry::fromBinary(CameraComponent& /*out*/, const char* /*data*/, size_t /*length*/)
    {
        // TODO: Cam component binary deserialization
        return (ASSUME(false, "TODO: Cam component binary deserialization"), 0);
    }

    template <>
    bool ComponentRegistry::toJson(
        const CameraComponent& component, rapidjson::Writer<rapidjson::StringBuffer>& writer, const EntitiesMap&)
    {
        writer.StartObject();

        const std::string typeString = projectionTypeToString(component.getProjectionType());

        writer.Key("type");
        writer.String(typeString.c_str(), static_cast<rapidjson::SizeType>(typeString.size()));

        writer.Key("perspective_fov");
        writer.Double(component.getFovY().raw());

        writer.Key("perspective_near");
        writer.Double(component.getPerspectiveNear());

        writer.Key("perspective_far");
        writer.Double(component.getPerspectiveFar());

        writer.Key("orthographic_size");
        writer.Double(component.getOrthographicSize());

        writer.Key("orthographic_near");
        writer.Double(component.getOrthographicNear());

        writer.Key("orthographic_far");
        writer.Double(component.getOrthographicFar());

        return CHECK(writer.EndObject(), "Unable to serialize camera component");
    }

    template <>
    bool ComponentRegistry::fromJson(CameraComponent& /*out*/, const rapidjson::Value& /*json*/)
    {
        // TODO: Cam component json deserialization
        return ASSUME(false, "TODO: Cam component json deserialization") && false;
    }
}
