#include "PantheonScripting/LuaTypeRegistry.h"
#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include <Quaternion.h>

#include <sol/state.hpp>

using namespace LibMath;

namespace PantheonScripting::Bindings
{
    void LuaMathBinder::bindQuaternion(sol::state& luaState)
    {
        static constexpr const char* typeName = "Quaternion";

        sol::constructors<
            Quaternion(),
            Quaternion(float),
            Quaternion(float, Vector3),
            Quaternion(Radian, Vector3),
            Quaternion(Degree, Vector3),
            Quaternion(const Quaternion&),
            Quaternion(float, float, float, float)
        > ctor;

        sol::usertype quatType = luaState.new_usertype<Quaternion>(
            typeName, ctor,
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const Quaternion& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const Quaternion& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            sol::meta_function::multiplication, sol::overload(
                sol::resolve<Quaternion(Quaternion, const Quaternion&)>(&operator*),
                sol::resolve<Quaternion(Quaternion, float)>(&operator*),
                sol::resolve<Quaternion(float, Quaternion)>(&operator*)
            ),
            sol::meta_function::division, sol::overload(
                sol::resolve<Quaternion(Quaternion, const Quaternion&)>(&operator/),
                sol::resolve<Quaternion(Quaternion, float)>(&operator/)
            ),
            "identity", sol::readonly_property(&Quaternion::identity),
            "fromEuler", sol::overload(
                sol::resolve<Quaternion(const Radian&, const Radian&, const Radian&, ERotationOrder)>(&Quaternion::fromEuler),
                [](const Degree& x, const Degree& y, const Degree& z, const ERotationOrder order)
                {
                    return Quaternion::fromEuler(x, y, z, order);
                }
            ),
            "toEuler", [](const Quaternion& self, const ERotationOrder rotationOrder)
            -> std::tuple<Radian, Radian, Radian>
            {
                TVector3<Radian> out = self.toEuler(rotationOrder);
                return { out.m_x, out.m_y, out.m_z };
            },
            "fromTo", &Quaternion::fromTo<float>,
            "toAngleAxis", [](const Quaternion& self)
            -> std::pair<Radian, Vector3>
            {
                Radian  angle;
                Vector3 axis;
                self.toAngleAxis(angle, axis);
                return { angle, axis };
            },
            "isUnit", sol::readonly_property(&Quaternion::isUnit),
            "isIdentity", sol::readonly_property([](const Quaternion& self)
            {
                return self == Quaternion::identity();
            }),
            "magnitude", sol::readonly_property(&Quaternion::magnitude),
            "magnitudeSquared", sol::readonly_property(&Quaternion::magnitudeSquared),
            "dot", &Quaternion::dot<float>,
            "conjugate", sol::readonly_property(&Quaternion::conjugate),
            "inverse", sol::readonly_property(&Quaternion::inverse),
            "normalize", &Quaternion::normalize,
            "normalized", sol::readonly_property(&Quaternion::normalized),
            "x", &Quaternion::m_x,
            "y", &Quaternion::m_y,
            "z", &Quaternion::m_z,
            "w", &Quaternion::m_w
        );

        quatType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<Quaternion>(typeName);
        return (void)typeInfo;
    }
}
