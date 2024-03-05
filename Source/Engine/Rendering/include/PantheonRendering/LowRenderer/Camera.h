#pragma once
#include "PantheonRendering/Core/Color.h"
#include "PantheonRendering/Core/Layer.h"
#include "PantheonRendering/Enums/ECullingMode.h"
#include "PantheonRendering/Geometry/Frustum.h"

#include <Matrix/Matrix4.h>

namespace PantheonRendering::LowRenderer
{
    class Camera
    {
    public:
        static constexpr uint8_t CLEAR_COLOR_OFFSET = 0;
        static constexpr uint8_t CLEAR_COLOR_BIT    = 1 << CLEAR_COLOR_OFFSET;

        static constexpr uint8_t CLEAR_DEPTH_OFFSET = CLEAR_COLOR_OFFSET + 1;
        static constexpr uint8_t CLEAR_DEPTH_BIT    = 1 << CLEAR_DEPTH_OFFSET;

        static constexpr uint8_t CLEAR_STENCIL_OFFSET = CLEAR_DEPTH_OFFSET + 1;
        static constexpr uint8_t CLEAR_STENCIL_BIT    = 1 << CLEAR_STENCIL_OFFSET;

        /**
         * \brief Creates a camera with the given projection and view matrices
         * \param projection The camera's projection matrix
         * \param view The camera's view matrix
         * \param cullingMode The camera's culling mode
         */
        explicit Camera(LibMath::Matrix4 projection, LibMath::Matrix4 view, Enums::ECullingMode cullingMode);

        /**
         * \brief Creates a copy of the given camera
         * \param other The camera to copy
         */
        Camera(const Camera& other) = default;

        /**
         * \brief Creates a move copy of the given camera
         * \param other The camera to move
         */
        Camera(Camera&& other) noexcept = default;

        /**
         * \brief Destroys the camera
         */
        ~Camera() = default;

        /**
         * \brief Assigns a copy of the given camera to the current one
         * \param other The camera to copy
         * \return A reference to the current camera
         */
        Camera& operator=(const Camera& other) = default;

        /**
         * \brief Moves the given camera into the current one
         * \param other The camera to move
         * \return A reference to the current camera
         */
        Camera& operator=(Camera&& other) noexcept = default;

        /**
         * \brief Gets the camera's view matrix
         * \return The camera's view matrix
         */
        const LibMath::Matrix4& getView() const;

        /**
         * \brief Sets the camera's view matrix
         * \param view The camera's new view matrix
         * \return A reference to the current camera
         */
        Camera& setView(const LibMath::Matrix4& view);

        /**
         * \brief Gets the camera's projection matrix
         * \return The camera's projection matrix
         */
        const LibMath::Matrix4& getProjection() const;

        /**
         * \brief Sets the camera's projection matrix
         * \param projection The camera's new projection matrix
         * \return A reference to the current camera
         */
        Camera& setProjection(const LibMath::Matrix4& projection);

        /**
         * \brief Gets the camera's view projection matrix
         * \return The camera's view projection matrix
         */
        const LibMath::Matrix4& getViewProjection() const;

        /**
         * \brief Gets the camera's frustum
         * \return The camera's frustum
         */
        const Geometry::Frustum& getFrustum() const;

        /**
         * \brief Gets the camera's culling mode
         * \return The camera's culling mode
         */
        Enums::ECullingMode getCullingMode() const;

        /**
         * \brief Sets the camera's culling mode
         * \param cullingMode The camera's new culling mode
         * \return A reference to the current camera
         */
        Camera& setCullingMode(Enums::ECullingMode cullingMode);

        /**
         * \brief Sets the camera's clear color
         * \param color The new clear color
         * \return A reference to the current camera
         */
        Camera& setClearColor(const Core::Color& color);

        /**
         * \brief Sets the camera's clear color
         * \param r The clear color's red component
         * \param g The clear color's green component
         * \param b The clear color's blue component
         * \param a The clear color's alpha component
         * \return A reference to the current camera
         */
        Camera& setClearColor(float r, float g, float b, float a = 1.f);

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
         */
        void setClearMask(uint8_t clearMask);

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
         */
        void setClearMask(bool clearColor, bool clearDepth, bool clearStencil);

        /**
         * \brief Gets the camera's culling mask
         * \return The camera's culling layer mask
         */
        Core::LayerMask getCullingMask() const;

        /**
         * \brief Sets the mask for the layers visible by the camera
         * \param cullingMask The camera's culling mask
         */
        void setCullingMask(Core::LayerMask cullingMask);

    private:
        LibMath::Matrix4  m_viewMatrix;
        LibMath::Matrix4  m_projectionMatrix;
        LibMath::Matrix4  m_viewProjectionMatrix;
        Geometry::Frustum m_frustum;

        Enums::ECullingMode m_cullingMode;

        uint8_t         m_clearMask   = CLEAR_COLOR_BIT | CLEAR_DEPTH_BIT | CLEAR_STENCIL_BIT;
        Core::Color     m_clearColor  = Core::Color::black;
        Core::LayerMask m_cullingMask = Core::Layer::ALL;

        /**
         * \brief Updates the camera's matrices and other cached information
         */
        void onChange();
    };
}
