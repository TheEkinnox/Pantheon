#include "PantheonRendering/Components/CameraComponent.h"

using namespace LibMath;
using namespace PantheonCore::Resources;
using namespace PantheonCore::Serialization;
using namespace PantheonRendering::Enums;
using namespace PantheonRendering::Core;

namespace PantheonRendering::Components
{
    const Camera& CameraComponent::operator*() const
    {
        return m_camera;
    }

    const Camera* CameraComponent::operator->() const
    {
        return &m_camera;
    }

    bool CameraComponent::toBinary(std::vector<char>& out) const
    {
        return m_target.toBinary(out)
            && CHECK(IByteSerializable::writeNumber(m_projectionType, out))
            && CHECK(IByteSerializable::writeNumber(m_cullingMode, out))
            && CHECK(IByteSerializable::writeNumber(m_clearMask, out))
            && CHECK(IByteSerializable::serializeVector4(m_clearColor, out))
            && CHECK(IByteSerializable::writeNumber(m_cullingMask, out))
            && CHECK(IByteSerializable::writeNumber(m_fovY.raw(), out))
            && CHECK(IByteSerializable::writeNumber(m_perspectiveNear, out))
            && CHECK(IByteSerializable::writeNumber(m_perspectiveFar, out))
            && CHECK(IByteSerializable::writeNumber(m_orthographicSize, out))
            && CHECK(IByteSerializable::writeNumber(m_orthographicNear, out))
            && CHECK(IByteSerializable::writeNumber(m_orthographicFar, out));
    }

    size_t CameraComponent::fromBinary(const char* data, size_t length)
    {
        if (!CHECK(data != nullptr && length > 0, "Unable to deserialize camera component - Empty buffer"))
            return 0;

        // Render target
        size_t offset = m_target.fromBinary(data, length);

        if (offset == 0)
            return 0;

        // Projection type
        if (!CHECK(length >= offset, "Unable to read camera component's projection type - Invalid offset"))
            return 0;

        size_t readBytes = IByteSerializable::readNumber(m_projectionType, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Failed to read camera component's projection type"))
            return 0;

        offset += readBytes;

        // Culling mode
        if (!CHECK(length >= offset, "Unable to read camera component's culling mode - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_cullingMode, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Failed to read camera component's culling mode"))
            return 0;

        offset += readBytes;

        // Clear mask
        if (!CHECK(length >= offset, "Unable to read camera component's clear mask - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_clearMask, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Failed to read camera component's clear mask"))
            return 0;

        offset += readBytes;

        // Clear color
        if (!CHECK(length >= offset, "Unable to read camera component's clear color - Invalid offset"))
            return 0;

        Vector4 colorVec;
        readBytes = IByteSerializable::deserializeVector4(colorVec, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Failed to read camera component's clear color"))
            return 0;

        m_clearColor = colorVec;
        offset += readBytes;

        // Culling mask
        if (!CHECK(length >= offset, "Unable to read camera component's culling mask - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_cullingMask, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Failed to read camera component's culling mask"))
            return 0;

        offset += readBytes;

        // Vertical fov
        if (!CHECK(length >= offset, "Unable to read camera component's vertical fov - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber<Radian, float>(m_fovY, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's vertical fov"))
            return 0;

        offset += readBytes;

        // Perspective near
        if (!CHECK(length >= offset, "Unable to read camera component's perspective near clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_perspectiveNear, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's perspective near clipping plane"))
            return 0;

        offset += readBytes;

        // Perspective far
        if (!CHECK(length >= offset, "Unable to read camera component's perspective far clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_perspectiveFar, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's perspective far clipping plane"))
            return 0;

        offset += readBytes;

        // Orthographic size
        if (!CHECK(length >= offset, "Unable to read camera component's orthographic size - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_orthographicSize, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's orthographic size"))
            return 0;

        offset += readBytes;

        // Orthographic near
        if (!CHECK(length >= offset, "Unable to read camera component's orthographic near clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_orthographicNear, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's orthographic near clipping plane"))
            return 0;

        offset += readBytes;

        // Orthographic far
        if (!CHECK(length >= offset, "Unable to read camera component's orthographic far clipping plane - Invalid offset"))
            return 0;

        readBytes = IByteSerializable::readNumber(m_orthographicFar, data + offset, length - offset);

        if (!CHECK(readBytes > 0, "Unable to read camera component's orthographic far clipping plane"))
            return 0;

        return offset + readBytes;
    }

    bool CameraComponent::toJson(JsonWriter& writer) const
    {
        writer.StartObject();

        const std::string typeString = projectionTypeToString(m_projectionType);

        writer.Key("type");
        writer.String(typeString.c_str(), static_cast<rapidjson::SizeType>(typeString.size()));

        writer.Key("target");
        if (!m_target.toJson(writer))
            return false;

        writer.Key("culling_mode");
        writer.Uint(static_cast<unsigned>(m_cullingMode));

        writer.Key("clear_mask");
        writer.Uint(m_clearMask);

        const std::string colorString = static_cast<Vector4>(m_clearColor).string();

        writer.Key("clear_color");
        writer.String(colorString.c_str(), static_cast<rapidjson::SizeType>(colorString.size()));

        writer.Key("culling_mask");
        writer.Uint(m_cullingMask);

        writer.Key("perspective_fov");
        writer.Double(m_fovY.degree());

        writer.Key("perspective_near");
        writer.Double(m_perspectiveNear);

        writer.Key("perspective_far");
        writer.Double(m_perspectiveFar);

        writer.Key("orthographic_size");
        writer.Double(m_orthographicSize);

        writer.Key("orthographic_near");
        writer.Double(m_orthographicNear);

        writer.Key("orthographic_far");
        writer.Double(m_orthographicFar);

        return CHECK(writer.EndObject(), "Unable to serialize camera component");
    }

    bool CameraComponent::fromJson(const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize camera component - Json value should be an object"))
            return false;

        auto it = json.FindMember("target");

        if (!CHECK(it != json.MemberEnd(), "Unable to deserialize camera component's target") || !m_target.fromJson(it->value))
            return false;

        it = json.FindMember("type");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize camera component's projection type"))
            return false;

        m_projectionType = stringToProjectionType({ it->value.GetString(), it->value.GetStringLength() });

        it = json.FindMember("culling_mode");

        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize camera component's culling mode"))
            return false;

        m_cullingMode = static_cast<ECullingMode>(it->value.GetUint());

        it = json.FindMember("clear_mask");

        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize camera component's clear mask"))
            return false;

        m_clearMask = static_cast<uint8_t>(it->value.GetUint());

        it = json.FindMember("clear_color");

        if (!CHECK(it != json.MemberEnd() && it->value.IsString(), "Unable to deserialize camera component's clear color"))
            return false;

        {
            std::istringstream iss(std::string(it->value.GetString(), it->value.GetStringLength()));

            Vector4 colorVec;
            iss >> colorVec;
            m_clearColor = colorVec;
        }

        it = json.FindMember("culling_mask");

        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize camera component's culling mask"))
            return false;

        m_cullingMask = static_cast<uint8_t>(it->value.GetUint());

        it = json.FindMember("perspective_fov");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize camera component's vertical fov"))
            return false;

        m_fovY = Degree(it->value.Get<float>());

        it = json.FindMember("perspective_near");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(),
                "Unable to deserialize camera component's perspective near clipping plane"))
            return false;

        m_perspectiveNear = it->value.Get<float>();

        it = json.FindMember("perspective_far");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(),
                "Unable to deserialize camera component's perspective far clipping plane"))
            return false;

        m_perspectiveFar = it->value.Get<float>();

        it = json.FindMember("orthographic_size");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(), "Unable to deserialize camera component's orthographic size"))
            return false;

        m_orthographicSize = it->value.Get<float>();

        it = json.FindMember("orthographic_near");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(),
                "Unable to deserialize camera component's orthographic near clipping plane"))
            return false;

        m_orthographicNear = it->value.Get<float>();

        it = json.FindMember("orthographic_far");

        if (!CHECK(it != json.MemberEnd() && it->value.IsNumber(),
                "Unable to deserialize camera component's orthographic far clipping plane"))
            return false;

        m_orthographicFar = it->value.Get<float>();
        return true;
    }

    void CameraComponent::recalculate(const Matrix4& view)
    {
        if (!m_isDirty)
            return;

        if (m_projectionType == EProjectionType::PERSPECTIVE)
        {
            const Matrix4 projMat = perspectiveProjection(m_fovY, m_aspect, m_perspectiveNear, m_perspectiveFar);
            m_camera.setViewProjection(projMat * view);
            m_isDirty = false;
            return;
        }

        const float   halfHeight = m_orthographicSize * .5f;
        const float   halfWidth  = halfHeight * m_aspect;
        const Matrix4 projMat    = orthographicProjection(-halfWidth, halfWidth, -halfHeight, halfHeight,
            m_orthographicNear, m_orthographicFar);

        m_camera.setViewProjection(projMat * view);
        m_isDirty = false;
    }

    ResourceRef<Resources::RenderTarget> CameraComponent::getTarget() const
    {
        return m_target;
    }

    EProjectionType CameraComponent::getProjectionType() const
    {
        return m_projectionType;
    }

    CameraComponent& CameraComponent::setProjectionType(const EProjectionType projectionType)
    {
        m_projectionType = projectionType;
        m_isDirty        = true;
        return *this;
    }

    ECullingMode CameraComponent::getCullingMode() const
    {
        return m_cullingMode;
    }

    CameraComponent& CameraComponent::setCullingMode(ECullingMode cullingMode)
    {
        m_cullingMode = cullingMode;
        m_isDirty     = true;
        return *this;
    }

    CameraComponent& CameraComponent::setClearColor(const Color& color)
    {
        m_clearColor = color;
        m_isDirty    = true;
        return *this;
    }

    CameraComponent& CameraComponent::setClearColor(const float r, const float g, const float b, const float a)
    {
        setClearColor({ r, g, b, a });
        m_isDirty = true;
        return *this;
    }

    Color CameraComponent::getClearColor() const
    {
        return m_clearColor;
    }

    uint8_t CameraComponent::getClearMask() const
    {
        return m_clearMask;
    }

    CameraComponent& CameraComponent::setClearMask(const uint8_t clearMask)
    {
        m_clearMask = clearMask;
        m_isDirty   = true;
        return *this;
    }

    void CameraComponent::getClearMask(bool& clearColor, bool& clearDepth, bool& clearStencil) const
    {
        clearColor   = m_clearMask & CLEAR_COLOR_BIT;
        clearDepth   = m_clearMask & CLEAR_DEPTH_BIT;
        clearStencil = m_clearMask & CLEAR_STENCIL_BIT;
    }

    CameraComponent& CameraComponent::setClearMask(bool clearColor, bool clearDepth, bool clearStencil)
    {
        m_clearMask = static_cast<uint8_t>(
            clearColor << CLEAR_COLOR_OFFSET |
            clearDepth << CLEAR_DEPTH_OFFSET |
            clearStencil << CLEAR_STENCIL_OFFSET
        );

        m_isDirty = true;
        return *this;
    }

    LayerMask CameraComponent::getCullingMask() const
    {
        return m_cullingMask;
    }

    CameraComponent& CameraComponent::setCullingMask(const LayerMask cullingMask)
    {
        m_cullingMask = cullingMask;
        m_isDirty     = true;
        return *this;
    }

    float CameraComponent::getAspect() const
    {
        return m_aspect;
    }

    CameraComponent& CameraComponent::setAspect(const float aspect)
    {
        m_aspect  = aspect;
        m_isDirty = true;
        return *this;
    }

    Radian CameraComponent::getFovY() const
    {
        return m_fovY;
    }

    CameraComponent& CameraComponent::setFovY(const Radian& fovY)
    {
        m_fovY    = fovY;
        m_isDirty = true;
        return *this;
    }

    float CameraComponent::getPerspectiveNear() const
    {
        return m_perspectiveNear;
    }

    CameraComponent& CameraComponent::setPerspectiveNear(const float zNear)
    {
        m_perspectiveNear = zNear;
        m_isDirty         = true;
        return *this;
    }

    float CameraComponent::getPerspectiveFar() const
    {
        return m_perspectiveFar;
    }

    CameraComponent& CameraComponent::setPerspectiveFar(const float zFar)
    {
        m_perspectiveFar = zFar;
        m_isDirty        = true;
        return *this;
    }

    CameraComponent& CameraComponent::setPerspective(const Radian& fovY, const float zNear, const float zFar)
    {
        m_projectionType  = EProjectionType::PERSPECTIVE;
        m_perspectiveNear = zNear;
        m_perspectiveFar  = zFar;
        m_fovY            = fovY;

        return *this;
    }

    float CameraComponent::getOrthographicSize() const
    {
        return m_orthographicSize;
    }

    CameraComponent& CameraComponent::setOrthographicSize(const float size)
    {
        m_orthographicSize = size;
        m_isDirty          = true;
        return *this;
    }

    float CameraComponent::getOrthographicNear() const
    {
        return m_orthographicNear;
    }

    CameraComponent& CameraComponent::setOrthographicNear(const float zNear)
    {
        m_orthographicNear = zNear;
        m_isDirty          = true;
        return *this;
    }

    float CameraComponent::getOrthographicFar() const
    {
        return m_orthographicFar;
    }

    CameraComponent& CameraComponent::setOrthographicFar(const float zFar)
    {
        m_orthographicFar = zFar;
        m_isDirty         = true;
        return *this;
    }

    CameraComponent& CameraComponent::setOrthographic(const float size, const float zNear, const float zFar)
    {
        m_projectionType   = EProjectionType::ORTHOGRAPHIC;
        m_orthographicSize = size;
        m_orthographicNear = zNear;
        m_orthographicFar  = zFar;

        m_isDirty = true;
        return *this;
    }
}
