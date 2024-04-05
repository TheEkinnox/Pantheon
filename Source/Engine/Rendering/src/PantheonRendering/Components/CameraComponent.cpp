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
    size_t ComponentRegistry::fromBinary(CameraComponent& out, const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize camera component - Empty buffer"))
            return 0;

        std::string typeString;
        size_t      offset = IByteSerializable::deserializeString(typeString, data, length);

        if (!CHECK(offset > 0, "Unable to deserialize camera component's projection type"))
            return 0;

        out.m_projectionType = stringToProjectionType(typeString);

        if (!CHECK(length >= offset, "Unable to read camera component's vertical fov - Invalid offset"))
            return 0;

        size_t readBytes = IByteSerializable::readNumber<Radian, float>(out.m_fovY, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's vertical fov"))
            return 0;

        offset += readBytes;

        if (!CHECK(length >= offset, "Unable to read camera component's perspective near clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(out.m_perspectiveNear, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's perspective near clipping plane"))
            return 0;

        offset += readBytes;

        if (!CHECK(length >= offset, "Unable to read camera component's perspective far clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(out.m_perspectiveFar, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's perspective far clipping plane"))
            return 0;

        offset += readBytes;

        if (!CHECK(length >= offset, "Unable to read camera component's orthographic size - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(out.m_orthographicSize, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's orthographic size"))
            return 0;

        offset += readBytes;

        if (!CHECK(length >= offset, "Unable to read camera component's orthographic near clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(out.m_orthographicNear, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's orthographic near clipping plane"))
            return 0;

        offset += readBytes;

        if (!CHECK(length >= offset, "Unable to read camera component's orthographic far clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(out.m_orthographicFar, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's orthographic far clipping plane"))
            return 0;

        out.recalculate();
        return offset + readBytes;
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
        writer.Double(component.getFovY().degree());

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
    bool ComponentRegistry::fromJson(CameraComponent& out, const rapidjson::Value& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize camera component - Json value should be an object"))
            return false;

        auto it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize camera component's projection type"))
            return false;

        out.m_projectionType = stringToProjectionType({ it->value.GetString(), it->value.GetStringLength() });

        it = json.FindMember("perspective_fov");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize camera component's vertical fov"))
            return false;

        out.m_fovY = Degree(it->value.Get<float>());

        it = json.FindMember("perspective_near");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(),
                "Unable to deserialize camera component's perspective near clipping plane"))
            return false;

        out.m_perspectiveNear = it->value.Get<float>();

        it = json.FindMember("perspective_far");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(),
                "Unable to deserialize camera component's perspective far clipping plane"))
            return false;

        out.m_perspectiveFar = it->value.Get<float>();

        it = json.FindMember("orthographic_size");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(), "Unable to deserialize camera component's orthographic size"))
            return false;

        out.m_orthographicSize = it->value.Get<float>();

        it = json.FindMember("orthographic_near");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(),
                "Unable to deserialize camera component's orthographic near clipping plane"))
            return false;

        out.m_orthographicNear = it->value.Get<float>();

        it = json.FindMember("orthographic_far");

        if (!CHECK(it != json.MemberEnd() && it->value.Is<float>(),
                "Unable to deserialize camera component's orthographic far clipping plane"))
            return false;

        out.m_orthographicFar = it->value.Get<float>();

        out.recalculate();
        return true;
    }
}
