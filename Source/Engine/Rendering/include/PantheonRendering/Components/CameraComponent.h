#pragma once
#include "PantheonCore/ECS/ComponentRegistry.h"

#include "PantheonRendering/Enums/EProjectionType.h"
#include "PantheonRendering/LowRenderer/Camera.h"

namespace PantheonRendering::Components
{
    class CameraComponent
    {
    public:
        /**
         * \brief Creates a default camera component
         */
        CameraComponent() = default;

        /**
         * \brief Creates a copy of the given camera component
         * \param other The camera component to copy
         */
        CameraComponent(const CameraComponent& other) = default;

        /**
         * \brief Creates a move copy of the given camera component
         * \param other The camera component to move
         */
        CameraComponent(CameraComponent&& other) noexcept = default;

        /**
         * \brief Destroys the camera component
         */
        ~CameraComponent() = default;

        /**
         * \brief Assigns a copy of the given camera component to this one
         * \param other The camera component to copy
         * \return A reference to the modified component
         */
        CameraComponent& operator=(const CameraComponent& other) = default;

        /**
         * \brief Moves the given camera component into this one
         * \param other The camera component to move
         * \return A reference to the modified component
         */
        CameraComponent& operator=(CameraComponent&& other) noexcept = default;

        /**
         * \brief Converts the camera component into a low level camera
         */
        operator LowRenderer::Camera() const;

        /**
         * \brief Recalculates the camera's projection matrix
         */
        void recalculate();

        /**
         * \brief Gets the camera's projection type
         * \return The camera's projection type
         */
        Enums::EProjectionType getProjectionType() const;

        /**
         * \brief Sets the camera's projection type
         * \param projectionType The camera's new projection type
         */
        void setProjectionType(Enums::EProjectionType projectionType);

        /**
         * \brief Gets the camera's aspect ratio
         * \return The camera's aspect ratio
         */
        float getAspect() const;

        /**
         * \brief Sets the camera's aspect ratio
         * \param aspect The camera's new aspect ratio
         */
        void setAspect(float aspect);

        /**
         * \brief Gets the camera's vertical field of view
         * \return The camera's vertical field of view
         */
        LibMath::Radian getFovY() const;

        /**
         * \brief Sets the camera's vertical field of view
         * \param fovY The camera's new vertical field of view
         */
        void setFovY(LibMath::Radian fovY);

        /**
         * \brief Gets the camera's perspective near clipping plane
         * \return The camera's perspective near clipping plane
         */
        float getPerspectiveNear() const;

        /**
         * \brief Sets the camera's perspective near clipping plane
         * \param zNear The camera's new perspective near clipping plane
         */
        void setPerspectiveNear(float zNear);

        /**
         * \brief Gets the camera's perspective far clipping plane
         * \return The camera's perspective far clipping plane
         */
        float getPerspectiveFar() const;

        /**
         * \brief Sets the camera's perspective far clipping plane
         * \param zFar The camera's new perspective far clipping plane
         */
        void setPerspectiveFar(float zFar);

        /**
         * \brief Sets the camera's field of view and its perspective near and far clipping planes
         * \param fovY The camera's new vertical field of view
         * \param zNear The camera's new near clipping plane
         * \param zFar The camera's new far clipping plane
         */
        void setPerspective(LibMath::Radian fovY, float zNear, float zFar);

        /**
         * \brief Gets the camera's orthographic size
         * \return The camera's orthographic size
         */
        float getOrthographicSize() const;

        /**
         * \brief Sets the camera's orthographic size
         * \param size The camera's new orthographic size
         */
        void setOrthographicSize(float size);

        /**
         * \brief Gets the camera's orthographic near clipping plane
         * \return The camera's orthographic near clipping plane
         */
        float getOrthographicNear() const;

        /**
         * \brief Sets the camera's orthographic near clipping plane
         * \param zNear The camera's new orthographic near clipping plane
         */
        void setOrthographicNear(float zNear);

        /**
         * \brief Gets the camera's orthographic far clipping plane
         * \return The camera's orthographic far clipping plane
         */
        float getOrthographicFar() const;

        /**
         * \brief Sets the camera's orthographic far clipping plane
         * \param zFar The camera's new orthographic far clipping plane
         */
        void setOrthographicFar(float zFar);

        /**
         * \brief Sets the camera's orthographic size and near/far clipping planes
         * \param size The camera's new orthographic size
         * \param zNear The camera's new orthographic near clipping plane
         * \param zFar The camera's new orthographic far clipping plane
         */
        void setOrthographic(float size, float zNear, float zFar);

    private:
        LowRenderer::Camera    m_camera;
        Enums::EProjectionType m_projectionType;

        LibMath::Radian m_fovY;
        float           m_perspectiveNear;
        float           m_perspectiveFar;

        float m_orthographicSize;
        float m_orthographicNear;
        float m_orthographicFar;

        float m_aspect;
    };
}

namespace PantheonCore::ECS
{
    template <>
    bool ComponentRegistry::toBinary(
        const PantheonRendering::Components::CameraComponent& component, std::vector<char>& out, const EntitiesMap&);

    template <>
    size_t ComponentRegistry::fromBinary(PantheonRendering::Components::CameraComponent& out, const char* data, size_t length);

    template <>
    bool ComponentRegistry::toJson(
        const PantheonRendering::Components::CameraComponent& component, rapidjson::Writer<rapidjson::StringBuffer>& writer,
        const EntitiesMap&);

    template <>
    bool ComponentRegistry::fromJson(PantheonRendering::Components::CameraComponent& out, const rapidjson::Value& json);
}
