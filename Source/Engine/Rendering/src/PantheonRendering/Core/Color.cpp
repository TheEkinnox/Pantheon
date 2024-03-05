#include "PantheonRendering/Core/Color.h"

using namespace LibMath;

namespace PantheonRendering::Core
{
    const Color Color::red       = { 1.f, 0.f, 0.f, 1.f };
    const Color Color::green     = { 0.f, 1.f, 0.f, 1.f };
    const Color Color::blue      = { 0.f, 0.f, 1.f, 1.f };
    const Color Color::cyan      = { 0.f, 1.f, 1.f, 1.f };
    const Color Color::magenta   = { 1.f, 0.f, 1.f, 1.f };
    const Color Color::yellow    = { 1.f, 1.f, 0.f, 1.f };
    const Color Color::skyBlue   = { .529f, .808f, .922f, 1.f };
    const Color Color::lime      = { .75f, 1.f, 0.f, 1.f };
    const Color Color::orange    = { .529f, .808f, .922f, 1.f };
    const Color Color::black     = { 0.f, 0.f, 0.f, 1.f };
    const Color Color::darkGray  = { .25f, .25f, .25f, 1.f };
    const Color Color::gray      = { .5f, .5f, .5f, 1.f };
    const Color Color::lightGray = { .75f, .75f, .75f, 1.f };
    const Color Color::white     = { 1.f, 1.f, 1.f, 1.f };
    const Color Color::clear     = { 0.f, 0.f, 0.f, 0.f };

    Color::Color(const float r, const float g, const float b)
        : Color(r, g, b, 1)
    {
    }

    Color::Color(const float r, const float g, const float b, const float a)
        : m_r(r), m_g(g), m_b(b), m_a(a)
    {
    }

    Color::Color(const Vector3& rgb)
        : Color(rgb.m_x, rgb.m_y, rgb.m_z)
    {
    }

    Color::Color(const Vector3& rgb, const float a)
        : Color(rgb.m_x, rgb.m_y, rgb.m_z, a)
    {
    }

    Color::Color(const Vector4& rgba)
        : Color(rgba.m_x, rgba.m_y, rgba.m_z, rgba.m_w)
    {
    }

    Color::operator Vector3() const
    {
        return { m_r, m_g, m_b };
    }

    Color::operator Vector4() const
    {
        return { m_r, m_g, m_b, m_a };
    }

    Color Color::operator+(const Color& other) const
    {
        return Vector4(*this) + Vector4(other);
    }

    Color Color::operator-(const Color& other) const
    {
        return Vector4(*this) - Vector4(other);
    }

    Color Color::operator*(const Color& other) const
    {
        return Vector4(*this) * Vector4(other);
    }

    Color Color::operator/(const Color& other) const
    {
        return Vector4(*this) / Vector4(other);
    }

    Color Color::operator*(const float scalar) const
    {
        return Vector4(*this) * Vector4(scalar);
    }

    Color Color::operator/(const float scalar) const
    {
        return *this / Vector4(scalar);
    }

    Color& Color::operator*=(const float scalar)
    {
        return *this *= Vector4(scalar);
    }

    Color& Color::operator/=(const float scalar)
    {
        return *this /= Vector4(scalar);
    }

    Color& Color::operator+=(const Color& other)
    {
        return *this = *this + other;
    }

    Color& Color::operator-=(const Color& other)
    {
        return *this = *this - other;
    }

    Color& Color::operator*=(const Color& other)
    {
        return *this = *this * other;
    }

    Color& Color::operator/=(const Color& other)
    {
        return *this = *this / other;
    }
}
