#pragma once
#include "PantheonRendering/Core/Camera.h"
#include "PantheonRendering/Core/Color.h"
#include "PantheonRendering/Core/Layer.h"
#include "PantheonRendering/Enums/ECullingMode.h"
#include "PantheonRendering/Enums/EProjectionType.h"
#include "PantheonRendering/Resources/RenderTarget.h"

#include <PantheonCore/ECS/ComponentRegistry.h>
#include <PantheonCore/Resources/ResourceRef.h>

namespace PantheonRendering::Components
{
    class CameraComponent
    {
    public:
        static constexpr uint8_t CLEAR_COLOR_OFFSET = 0;
        static constexpr uint8_t CLEAR_COLOR_BIT    = 1 << CLEAR_COLOR_OFFSET;

        static constexpr uint8_t CLEAR_DEPTH_OFFSET = CLEAR_COLOR_OFFSET + 1;
        static constexpr uint8_t CLEAR_DEPTH_BIT    = 1 << CLEAR_DEPTH_OFFSET;

        static constexpr uint8_t CLEAR_STENCIL_OFFSET = CLEAR_DEPTH_OFFSET + 1;
        static constexpr uint8_t CLEAR_STENCIL_BIT    = 1 << CLEAR_STENCIL_OFFSET;

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
        const Core::Camera& operator*() const;

        /**
         * \brief Converts the camera component into a low level camera
         */
        const Core::Camera* operator->() const;

        /**
         * \brief Serializes the camera component to binary
         * \param out The output memory buffer
         * \return True on success. False otherwise
         */
        bool toBinary(std::vector<char>& out) const;

        /**
         * \brief Deserializes the camera component from binary
         * \param data The input memory buffer
         * \param length The memory buffer's length
         * \return The number of read bytes on success. 0 otherwise
         */
        size_t fromBinary(const char* data, size_t length);

        /**
         * \brief Serializes the camera component to json
         * \param writer The output json writer
         * \return True on success. False otherwise
         */
        bool toJson(PantheonCore::Serialization::JsonWriter& writer) const;

        /**
         * \brief Deserializes the camera component from json
         * \param json The input json value
         * \return True on success. False otherwise
         */
        bool fromJson(const PantheonCore::Serialization::JsonValue& json);

        /**
         * \brief Recalculates the camera's projection matrix
         * \param view The camera's view matrix
         */
        void recalculate(const LibMath::Matrix4& view);

        /**
         * \brief Gets the camera's render target
         * \return The camera's render target
         */
        PantheonCore::Resources::ResourceRef<Resources::RenderTarget> getTarget() const;

        /**
         * \brief Gets the camera's projection type
         * \return The camera's projection type
         */
        Enums::EProjectionType getProjectionType() const;

        /**
         * \brief Sets the camera's projection type
         * \param projectionType The camera's new projection type
         * \return A reference to the modified component
         */
        CameraComponent& setProjectionType(Enums::EProjectionType projectionType);

        /**
         * \brief Gets the camera's culling mode
         * \return The camera's culling mode
         */
        Enums::ECullingMode getCullingMode() const;

        /**
         * \brief Sets the camera's culling mode
         * \param cullingMode The camera's new culling mode
         * \return A reference to the modified component
         */
        CameraComponent& setCullingMode(Enums::ECullingMode cullingMode);

        /**
         * \brief Sets the camera's clear color
         * \param color The new clear color
         * \return A reference to the modified component
         */
        CameraComponent& setClearColor(const Core::Color& color);

        /**
         * \brief Sets the camera's clear color
         * \param r The clear color's red component
         * \param g The clear color's green component
         * \param b The clear color's blue component
         * \param a The clear color's alpha component
         * \return A reference to the modified component
         */
        CameraComponent& setClearColor(float r, float g, float b, float a = 1.f);

        /**
         * \brief Gets the camera's current clear color
         * \return The camera's current clear color
         */
        Core::Color getClearColor() const;

        /**
         * \brief Gets the camera's buffer clearing mask
         * \return The camera's clear mask
         */
        uint8_t getClearMask() const;

        /**
         * \brief Sets the camera's buffer clearing mask
         * \param clearMask The buffer clear mask (any of CLEAR_COLOR_BIT, CLEAR_DEPTH_BIT and CLEAR_STENCIL_BIT)
         * \return A reference to the modified component
         */
        CameraComponent& setClearMask(uint8_t clearMask);

        /**
         * \brief Breaks the buffer clearing mask into separate values
         * \param clearColor The output target for whether the color buffer should be cleared
         * \param clearDepth The output target for whether the depth buffer should be cleared
         * \param clearStencil The output target for whether the stencil buffer should be cleared
         */
        void getClearMask(bool& clearColor, bool& clearDepth, bool& clearStencil) const;

        /**
         * \brief Updates the camera's clear mask from the given values
         * \param clearColor Whether the color buffer should be cleared
         * \param clearDepth Whether the depth buffer should be cleared
         * \param clearStencil Whether the stencil buffer should be cleared
         * \return A reference to the modified component
         */
        CameraComponent& setClearMask(bool clearColor, bool clearDepth, bool clearStencil);

        /**
         * \brief Gets the camera's culling mask
         * \return The camera's culling layer mask
         */
        Core::LayerMask getCullingMask() const;

        /**
         * \brief Sets the mask for the layers visible by the camera
         * \param cullingMask The camera's culling mask
         * \return A reference to the modified component
         */
        CameraComponent& setCullingMask(Core::LayerMask cullingMask);

        /**
         * \brief Gets the camera's aspect ratio
         * \return The camera's aspect ratio
         */
        float getAspect() const;

        /**
         * \brief Sets the camera's aspect ratio
         * \param aspect The camera's new aspect ratio
         * \return A reference to the modified component
         */
        CameraComponent& setAspect(float aspect);

        /**
         * \brief Gets the camera's vertical field of view
         * \return The camera's vertical field of view
         */
        LibMath::Radian getFovY() const;

        /**
         * \brief Sets the camera's vertical field of view
         * \param fovY The camera's new vertical field of view
         * \return A reference to the modified component
         */
        CameraComponent& setFovY(const LibMath::Radian& fovY);

        /**
         * \brief Gets the camera's perspective near clipping plane
         * \return The camera's perspective near clipping plane
         */
        float getPerspectiveNear() const;

        /**
         * \brief Sets the camera's perspective near clipping plane
         * \param zNear The camera's new perspective near clipping plane
         * \return A reference to the modified component
         */
        CameraComponent& setPerspectiveNear(float zNear);

        /**
         * \brief Gets the camera's perspective far clipping plane
         * \return The camera's perspective far clipping plane
         */
        float getPerspectiveFar() const;

        /**
         * \brief Sets the camera's perspective far clipping plane
         * \param zFar The camera's new perspective far clipping plane
         * \return A reference to the modified component
         */
        CameraComponent& setPerspectiveFar(float zFar);

        /**
         * \brief Sets the camera's field of view and its perspective near and far clipping planes
         * \param fovY The camera's new vertical field of view
         * \param zNear The camera's new near clipping plane
         * \param zFar The camera's new far clipping plane
         * \return A reference to the modified component
         */
        CameraComponent& setPerspective(const LibMath::Radian& fovY, float zNear, float zFar);

        /**
         * \brief Gets the camera's orthographic size
         * \return The camera's orthographic size
         */
        float getOrthographicSize() const;

        /**
         * \brief Sets the camera's orthographic size
         * \param size The camera's new orthographic size
         * \return A reference to the modified component
         */
        CameraComponent& setOrthographicSize(float size);

        /**
         * \brief Gets the camera's orthographic near clipping plane
         * \return The camera's orthographic near clipping plane
         */
        float getOrthographicNear() const;

        /**
         * \brief Sets the camera's orthographic near clipping plane
         * \param zNear The camera's new orthographic near clipping plane
         * \return A reference to the modified component
         */
        CameraComponent& setOrthographicNear(float zNear);

        /**
         * \brief Gets the camera's orthographic far clipping plane
         * \return The camera's orthographic far clipping plane
         */
        float getOrthographicFar() const;

        /**
         * \brief Sets the camera's orthographic far clipping plane
         * \param zFar The camera's new orthographic far clipping plane
         * \return A reference to the modified component
         */
        CameraComponent& setOrthographicFar(float zFar);

        /**
         * \brief Sets the camera's orthographic size and near/far clipping planes
         * \param size The camera's new orthographic size
         * \param zNear The camera's new orthographic near clipping plane
         * \param zFar The camera's new orthographic far clipping plane
         * \return A reference to the modified component
         */
        CameraComponent& setOrthographic(float size, float zNear, float zFar);

    private:
        Core::Camera m_camera;

        PantheonCore::Resources::ResourceRef<Resources::RenderTarget> m_target;

        Enums::EProjectionType m_projectionType;
        Enums::ECullingMode    m_cullingMode;

        uint8_t         m_clearMask   = CLEAR_COLOR_BIT | CLEAR_DEPTH_BIT | CLEAR_STENCIL_BIT;
        Core::Color     m_clearColor  = Core::Color::black;
        Core::LayerMask m_cullingMask = Core::Layer::ALL;

        LibMath::Radian m_fovY;
        float           m_perspectiveNear;
        float           m_perspectiveFar;

        float m_orthographicSize;
        float m_orthographicNear;
        float m_orthographicFar;

        float m_aspect;
        bool  m_isDirty;
    };
}
