#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include "PantheonScripting/LuaTypeRegistry.h"

#include <Vector.h>

#include <sol/state.hpp>

using namespace LibMath;

namespace PantheonScripting::Bindings
{
    void LuaMathBinder::bindVector2(sol::state& luaState)
    {
        static constexpr const char* typeName = "Vector2";

        sol::constructors<
            Vector2(),
            Vector2(float),
            Vector2(float, float),
            Vector2(const Vector2&)
        > ctor;

        sol::usertype vectorType = luaState.new_usertype<Vector2>(
            typeName, ctor,
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const Vector2& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const Vector2& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            sol::meta_function::unary_minus, sol::resolve<Vector2(const Vector2&)>(&operator-),
            sol::meta_function::addition, sol::overload(
                sol::resolve<Vector2(Vector2, const Vector2&)>(&operator+),
                sol::resolve<Vector2(Vector2, float)>(&operator+)
            ),
            sol::meta_function::subtraction, sol::overload(
                sol::resolve<Vector2(Vector2, const Vector2&)>(&operator-),
                sol::resolve<Vector2(Vector2, float)>(&operator-)
            ),
            sol::meta_function::multiplication, sol::overload(
                sol::resolve<Vector2(Vector2, const Vector2&)>(&operator*),
                sol::resolve<Vector2(Vector2, float)>(&operator*)
            ),
            sol::meta_function::division, sol::overload(
                sol::resolve<Vector2(Vector2, const Vector2&)>(&operator/),
                sol::resolve<Vector2(Vector2, float)>(&operator/)
            ),
            "zero", sol::readonly_property(&Vector2::zero),
            "one", sol::readonly_property(&Vector2::one),
            "up", sol::readonly_property(&Vector2::up),
            "down", sol::readonly_property(&Vector2::down),
            "left", sol::readonly_property(&Vector2::left),
            "right", sol::readonly_property(&Vector2::right),
            "angleFrom", &Vector2::angleFrom<float>,
            "signedAngleFrom", &Vector2::signedAngleFrom<float>,
            "cross", &Vector2::cross<float>,
            "distanceFrom", &Vector2::distanceFrom<float>,
            "distanceSquaredFrom", &Vector2::distanceSquaredFrom<float>,
            "dot", &Vector2::dot<float>,
            "isUnit", sol::readonly_property(&Vector2::isUnitVector),
            "magnitude", sol::readonly_property(&Vector2::magnitude),
            "magnitudeSquared", sol::readonly_property(&Vector2::magnitudeSquared),
            "normalize", &Vector2::normalize,
            "normalized", sol::readonly_property(&Vector2::normalized),
            "projectOnto", &Vector2::projectOnto,
            "reflectOnto", &Vector2::reflectOnto,
            "rotate", &Vector2::rotate,
            "rotated", [](Vector2 self, const Radian& angle)
            {
                self.rotate(angle);
                return self;
            },
            "scale", &Vector2::scale,
            "translate", &Vector2::translate,
            "x", &Vector2::m_x,
            "y", &Vector2::m_y
        );

        vectorType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<Vector2>(typeName);
        return (void)typeInfo;
    }

    void LuaMathBinder::bindVector3(sol::state& luaState)
    {
        static constexpr const char* typeName = "Vector3";

        sol::constructors<
            Vector3(),
            Vector3(float),
            Vector3(const Vector2&),
            Vector3(const Vector3&),
            Vector3(float, float, float)
        > ctor;

        sol::usertype vectorType = luaState.new_usertype<Vector3>(
            typeName, ctor,
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const Vector3& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const Vector3& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            sol::meta_function::unary_minus, sol::resolve<Vector3(const Vector3&)>(&operator-),
            sol::meta_function::addition, sol::overload(
                sol::resolve<Vector3(Vector3, const Vector3&)>(&operator+),
                sol::resolve<Vector3(Vector3, float)>(&operator+)
            ),
            sol::meta_function::subtraction, sol::overload(
                sol::resolve<Vector3(Vector3, const Vector3&)>(&operator-),
                sol::resolve<Vector3(Vector3, float)>(&operator-)
            ),
            sol::meta_function::multiplication, sol::overload(
                sol::resolve<Vector3(Vector3, const Vector3&)>(&operator*),
                sol::resolve<Vector3(Vector3, float)>(&operator*)
            ),
            sol::meta_function::division, sol::overload(
                sol::resolve<Vector3(Vector3, const Vector3&)>(&operator/),
                sol::resolve<Vector3(Vector3, float)>(&operator/)
            ),
            "zero", sol::readonly_property(&Vector3::zero),
            "one", sol::readonly_property(&Vector3::one),
            "up", sol::readonly_property(&Vector3::up),
            "down", sol::readonly_property(&Vector3::down),
            "left", sol::readonly_property(&Vector3::left),
            "right", sol::readonly_property(&Vector3::right),
            "front", sol::readonly_property(&Vector3::front),
            "back", sol::readonly_property(&Vector3::back),
            "xy", [](const Vector3& self)
            {
                return Vector2(self.m_x, self.m_y);
            },
            "angleFrom", &Vector3::angleFrom,
            "signedAngleFrom", &Vector3::signedAngleFrom,
            "cross", &Vector3::cross,
            "distanceFrom", &Vector3::distanceFrom,
            "distanceSquaredFrom", &Vector3::distanceSquaredFrom,
            "distance2DFrom", &Vector3::distance2DFrom,
            "distance2DSquaredFrom", &Vector3::distance2DSquaredFrom,
            "dot", &Vector3::dot<float>,
            "isUnit", sol::readonly_property(&Vector3::isUnitVector),
            "magnitude", sol::readonly_property(&Vector3::magnitude),
            "magnitudeSquared", sol::readonly_property(&Vector3::magnitudeSquared),
            "normalize", &Vector3::normalize,
            "normalized", sol::readonly_property(&Vector3::normalized),
            "projectOnto", &Vector3::projectOnto,
            "reflectOnto", &Vector3::reflectOnto,
            "rotate", sol::overload(
                static_cast<void (Vector3::*)(const Radian&, const Vector3&)>(&Vector3::rotate),
                static_cast<void (Vector3::*)(const Quaternion&)>(&Vector3::rotate)
            ),
            "rotated", sol::overload(
                [](Vector3 self, const Radian& angle, const Vector3& axis)
                {
                    self.rotate(angle, axis);
                    return self;
                },
                [](Vector3 self, const Quaternion& quaternion)
                {
                    self.rotate(quaternion);
                    return self;
                }
            ),
            "scale", &Vector3::scale,
            "translate", &Vector3::translate,
            "x", &Vector3::m_x,
            "y", &Vector3::m_y,
            "z", &Vector3::m_z
        );

        vectorType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<Vector3>(typeName);
        return (void)typeInfo;
    }

    void LuaMathBinder::bindVector4(sol::state& luaState)
    {
        static constexpr const char* typeName = "Vector4";

        sol::constructors<
            Vector4(),
            Vector4(float),
            Vector4(const Vector4&),
            Vector4(const Vector3&, float),
            Vector4(float, float, float, float)
        > ctor;

        sol::usertype vectorType = luaState.new_usertype<Vector4>(
            typeName, ctor,
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const Vector4& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const Vector4& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            sol::meta_function::unary_minus, sol::resolve<Vector4(const Vector4&)>(&operator-),
            sol::meta_function::addition, sol::overload(
                sol::resolve<Vector4(Vector4, const Vector4&)>(&operator+),
                sol::resolve<Vector4(Vector4, float)>(&operator+)
            ),
            sol::meta_function::subtraction, sol::overload(
                sol::resolve<Vector4(Vector4, const Vector4&)>(&operator-),
                sol::resolve<Vector4(Vector4, float)>(&operator-)
            ),
            sol::meta_function::multiplication, sol::overload(
                sol::resolve<Vector4(Vector4, const Vector4&)>(&operator*),
                sol::resolve<Vector4(Vector4, float)>(&operator*)
            ),
            sol::meta_function::division, sol::overload(
                sol::resolve<Vector4(Vector4, const Vector4&)>(&operator/),
                sol::resolve<Vector4(Vector4, float)>(&operator/)
            ),
            "zero", sol::readonly_property(&Vector4::zero),
            "one", sol::readonly_property(&Vector4::one),
            "up", sol::readonly_property(&Vector4::up),
            "down", sol::readonly_property(&Vector4::down),
            "left", sol::readonly_property(&Vector4::left),
            "right", sol::readonly_property(&Vector4::right),
            "front", sol::readonly_property(&Vector4::front),
            "back", sol::readonly_property(&Vector4::back),
            "xy", sol::readonly_property(
                [](const Vector4& self)
                {
                    return Vector2(self.m_x, self.m_y);
                }
            ),
            "xyz", sol::readonly_property(&Vector4::xyz),
            "distanceFrom", &Vector4::distanceFrom<float>,
            "distanceSquaredFrom", &Vector4::distanceSquaredFrom<float>,
            "dot", &Vector4::dot<float>,
            "isUnit", sol::readonly_property(&Vector4::isUnitVector),
            "magnitude", sol::readonly_property(&Vector4::magnitude),
            "magnitudeSquared", sol::readonly_property(&Vector4::magnitudeSquared),
            "normalize", &Vector4::normalize,
            "normalized", sol::readonly_property(&Vector4::normalized),
            "rotate", sol::overload(
                static_cast<void (Vector4::*)(const Radian&, const Vector3&)>(&Vector4::rotate),
                static_cast<void (Vector4::*)(const Quaternion&)>(&Vector4::rotate)
            ),
            "rotated", sol::overload(
                [](Vector4 self, const Radian& angle, const Vector3& axis)
                {
                    self.rotate(angle, axis);
                    return self;
                },
                [](Vector4 self, const Quaternion& quaternion)
                {
                    self.rotate(quaternion);
                    return self;
                }
            ),
            "scale", &Vector4::scale,
            "translate", &Vector4::translate,
            "x", &Vector4::m_x,
            "y", &Vector4::m_y,
            "z", &Vector4::m_z,
            "w", &Vector4::m_w
        );

        vectorType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<Vector4>(typeName);
        return (void)typeInfo;
    }
}
