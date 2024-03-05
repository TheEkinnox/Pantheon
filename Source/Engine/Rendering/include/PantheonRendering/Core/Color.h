#pragma once
#include <Vector/Vector3.h>
#include <Vector/Vector4.h>

namespace PantheonRendering::Core
{
    struct Color
    {
        float m_r = 1;
        float m_g = 0;
        float m_b = 1;
        float m_a = 1;

        static const Color red;
        static const Color green;
        static const Color blue;
        static const Color cyan;
        static const Color magenta;
        static const Color yellow;
        static const Color lime;
        static const Color skyBlue;
        static const Color orange;
        static const Color black;
        static const Color darkGray;
        static const Color gray;
        static const Color lightGray;
        static const Color white;
        static const Color clear;

        Color() = default;

        /**
         * \brief Creates a copy of the given color
         * \param other The color to copy
         */
        Color(const Color& other) = default;

        /**
         * \brief Creates a move copy of the given color
         * \param other The color to move
         */
        Color(Color&& other) = default;

        /**
         * \brief Creates a color with the given red, green and blue values
         * \param r The color's red value
         * \param g The color's green value
         * \param b The color's blue value
         */
        Color(float r, float g, float b);

        /**
         * \brief Creates a color with the given red, green, blue and alpha values
         * \param r The color's red value
         * \param g The color's green value
         * \param b The color's blue value
         * \param a The color's alpha value
         */
        Color(float r, float g, float b, float a);

        /**
         * \brief Creates a color from the given vector (x=r, y=g, z=b)
         * \param rgb The source color vector
         */
        Color(const LibMath::Vector3& rgb);

        /**
         * \brief Creates a color from the given vector and alpha value (x=r, y=g, z=b)
         * \param rgb The source color vector
         * \param a The color's alpha
         */
        Color(const LibMath::Vector3& rgb, float a);

        /**
         * \brief Creates a color from the given vector (x=r, y=g, z=b, w=a)
         * \param rgba The source color vector
         */
        Color(const LibMath::Vector4& rgba);

        ~Color() = default;

        /**
         * \brief Assigns a copy of the given color to this one
         * \param other The color to copy
         * \return The modifier color
         */
        Color& operator=(const Color& other) = default;

        /**
         * \brief Moves the given color into this one
         * \param other The color to move
         * \return The modifier color
         */
        Color& operator=(Color&& other) = default;

        /**
         * \brief Casts the color to a vector3
         * \return The r, g and b components of the color as a vector3
         */
        operator LibMath::Vector3() const;

        /**
         * \brief Casts the color to a vector4
         * \return The r, g and b and a components of the color as a vector4
         */
        operator LibMath::Vector4() const;

        Color operator+(const Color& other) const;
        Color operator-(const Color& other) const;
        Color operator*(const Color& other) const;
        Color operator/(const Color& other) const;
        Color operator*(float scalar) const;
        Color operator/(float scalar) const;

        Color& operator-=(const Color& other);
        Color& operator*=(const Color& other);
        Color& operator+=(const Color& other);
        Color& operator/=(const Color& other);
        Color& operator*=(float scalar);
        Color& operator/=(float scalar);
    };
}
