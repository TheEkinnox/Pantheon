#pragma once
#include "PantheonRendering/Core/Color.h"

#include <Matrix/Matrix4.h>

#include <Vector/Vector3.h>

namespace PantheonRendering::Core
{
    struct Light
    {
        /**
         * \brief The light's color (alpha is intensity)
         */
        Color m_color = Color::white;

        Light() = default;
        Light(const Color& color);

        Light(const Light&) = default;
        Light(Light&&)      = default;

        virtual ~Light() = default;

        Light& operator=(const Light&) = default;
        Light& operator=(Light&&)      = default;

        /**
         * \brief Creates a matrix storing the light's color in the first column,\n
         and the light type in the fourth column's fourth row
         * \return A matrix containing the light's data
         */
        virtual LibMath::Matrix4 getMatrix() const;
    };

    struct DirectionalLight final : Light
    {
        LibMath::Vector3 m_direction;

        DirectionalLight() = default;
        DirectionalLight(const Light& light, const LibMath::Vector3& direction);

        DirectionalLight(const DirectionalLight&) = default;
        DirectionalLight(DirectionalLight&&)      = default;
        ~DirectionalLight() override              = default;

        DirectionalLight& operator=(const DirectionalLight&) = default;
        DirectionalLight& operator=(DirectionalLight&&)      = default;

        /**
         * \brief Creates a matrix storing the light's color in the first column,\n
         the direction in the remaining space of the first row,\n
         and the light type in the fourth column's fourth row
         * \return A matrix containing the light's data
         */
        LibMath::Matrix4 getMatrix() const override;
    };

    struct Attenuation
    {
        float m_constant  = 1;
        float m_linear    = 0;
        float m_quadratic = 0;

        Attenuation() = default;
        explicit Attenuation(float range);
        Attenuation(float constant, float linear, float quadratic);
    };

    struct PointLight final : Light
    {
        LibMath::Vector3 m_position;
        Attenuation      m_attenuation;

        PointLight() = default;
        PointLight(const Light& light, const LibMath::Vector3& position, const Attenuation& attenuation);

        PointLight(const PointLight&) = default;
        PointLight(PointLight&&)      = default;
        ~PointLight() override        = default;

        PointLight& operator=(const PointLight&) = default;
        PointLight& operator=(PointLight&&)      = default;

        /**
         * \brief Creates a matrix storing the light's color in the first column,\n
         the position in the remaining space of the second row,\n
         the attenuation data in the remaining space of the third row,\n
         and the light type in the fourth column's fourth row
         * \return A matrix containing the light's data
         */
        LibMath::Matrix4 getMatrix() const override;
    };

    struct Cutoff
    {
        float m_inner;
        float m_outer;
    };

    struct SpotLight final : Light
    {
        LibMath::Vector3 m_position;
        LibMath::Vector3 m_direction;

        Attenuation m_attenuation;
        Cutoff      m_cutoff{ 0, 0 };

        SpotLight() = default;
        SpotLight(const Light&       light, const LibMath::Vector3& position, const LibMath::Vector3& direction,
                  const Attenuation& attenuation, const Cutoff&     cutoff);

        SpotLight(const SpotLight&) = default;
        SpotLight(SpotLight&&)      = default;
        ~SpotLight() override       = default;

        SpotLight& operator=(const SpotLight&) = default;
        SpotLight& operator=(SpotLight&&)      = default;

        /**
         * \brief Creates a matrix storing the light's color in the first column,\n
         the direction in the remaining space of the first row,\n
         the position in the remaining space of the second row,\n
         the attenuation data in the remaining space of the third row,\n
         the cutoff data in the remaining space of the fourth row,\n
         and the light type in the fourth column's fourth row
         * \return A matrix containing the light's data
         */
        LibMath::Matrix4 getMatrix() const override;
    };
}
