#include "PantheonScripting/LuaContext.h"
#include "PantheonScripting/LuaTypeRegistry.h"
#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include <Transform.h>

#include <sol/state.hpp>

using namespace LibMath;
using namespace PantheonCore::Serialization;

namespace PantheonScripting::Bindings
{
    void LuaMathBinder::bindTransform(sol::state& luaState)
    {
        static constexpr const char* typeName = "Transform";

        sol::constructors<
            Transform(),
            Transform(Matrix4x4),
            Transform(const Vector3&, const Quaternion&, const Vector3&),
            Transform(const Transform&)
        > ctor;

        sol::usertype transformType = luaState.new_usertype<Transform>(
            typeName, ctor,
            sol::meta_function::to_string, [](const Transform& self)
            {
                std::ostringstream oss;
                oss << '{' << self.getPosition() << ',' << self.getRotation() << ',' << self.getScale() << '}';
                return oss.str();
            },
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const Transform& self)
                {
                    std::ostringstream oss;
                    oss << str << '{' << self.getPosition() << ',' << self.getRotation() << ',' << self.getScale() << '}';
                    return oss.str();
                },
                [](const Transform& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << '{' << self.getPosition() << ',' << self.getRotation() << ',' << self.getScale() << '}' << str;
                    return oss.str();
                }
            ),
            "hasParent", &Transform::hasParent,
            "parent", sol::property(
                sol::resolve<Transform*()>(&Transform::getParent),
                [](Transform& self, Transform* parent)
                {
                    (void)self.setParent(parent);
                    return self;
                }
            ),
            "setParent", sol::overload(
                &Transform::setParent,
                [](Transform& self, Transform* parent)
                {
                    return self.setParent(parent);
                }
            ),
            /* Local */
            "forward", sol::readonly_property(&Transform::forward),
            "back", sol::readonly_property(&Transform::back),
            "right", sol::readonly_property(&Transform::right),
            "left", sol::readonly_property(&Transform::left),
            "up", sol::readonly_property(&Transform::up),
            "down", sol::readonly_property(&Transform::down),
            "position", sol::property(&Transform::getPosition, &Transform::setPosition),
            "rotation", sol::property(&Transform::getRotation, &Transform::setRotation),
            "scale", sol::property(&Transform::getScale, &Transform::setScale),
            "matrix", sol::property(&Transform::getMatrix, &Transform::setMatrix),
            "inverse", sol::readonly_property(&Transform::inverse),
            "getEuler", [](const Transform& self, const ERotationOrder order)
            -> std::tuple<Radian, Radian, Radian>
            {
                TVector3<Radian> out = self.getEuler(order);
                return { out.m_x, out.m_y, out.m_z };
            },
            "setEuler", [](
            Transform& self, const Radian& x, const Radian& y, const Radian& z, const ERotationOrder rotationOrder)
            {
                return self.setEuler({ x, y, z }, rotationOrder);
            },
            "getAll", [](const Transform& self)
            -> std::tuple<Vector3, Quaternion, Vector3>
            {
                return { self.getPosition(), self.getRotation(), self.getScale() };
            },
            "setAll", &Transform::setAll,
            "translate", &Transform::translate,
            "rotate", sol::resolve<Transform&(const Quaternion&)>(&Transform::rotate),
            "scale", &Transform::scale,
            "invert", &Transform::invert,
            /* World */
            "worldForward", sol::readonly_property(&Transform::worldForward),
            "worldBack", sol::readonly_property(&Transform::worldBack),
            "worldRight", sol::readonly_property(&Transform::worldRight),
            "worldLeft", sol::readonly_property(&Transform::worldLeft),
            "worldUp", sol::readonly_property(&Transform::worldUp),
            "worldDown", sol::readonly_property(&Transform::worldDown),
            "worldPosition", sol::property(&Transform::getWorldPosition, &Transform::setWorldPosition),
            "worldRotation", sol::property(&Transform::getWorldRotation, &Transform::setWorldRotation),
            "worldScale", sol::property(&Transform::getWorldScale, &Transform::setWorldScale),
            "worldMatrix", sol::property(&Transform::getWorldMatrix, &Transform::setWorldMatrix),
            "worldInverse", sol::readonly_property(&Transform::inverseWorld),
            "getWorldEuler", [](const Transform& self, const ERotationOrder order)
            -> std::tuple<Radian, Radian, Radian>
            {
                TVector3<Radian> out = self.getWorldEuler(order);
                return { out.m_x, out.m_y, out.m_z };
            },
            "setWorldEuler", [](Transform& self, const Radian& x, const Radian& y, const Radian& z, const ERotationOrder order)
            {
                return self.setWorldEuler({ x, y, z }, order);
            },
            "getAllWorld", [](const Transform& self)
            -> std::tuple<Vector3, Quaternion, Vector3>
            {
                return { self.getWorldPosition(), self.getWorldRotation(), self.getWorldScale() };
            },
            "setAllWorld", &Transform::setAllWorld,
            "translateWorld", &Transform::worldTranslate,
            "rotateWorld", sol::resolve<Transform&(const Quaternion&)>(&Transform::worldRotate),
            "scaleWorld", &Transform::worldScale,
            "invertWorld", &Transform::invertWorld
        );

        {
            sol::table meta = luaState.get<sol::table>(typeName);
            meta.set("interpolate", &Transform::interpolate);
            meta.set("interpolateWorld", &Transform::interpolateWorld);
            meta.set("generateMatrix", &Transform::generateMatrix);
            meta.set("decomposeMatrix", [](const Matrix4x4& matrix)
                -> std::tuple<Vector3, Quaternion, Vector3>
                {
                    Vector3    position, scale;
                    Quaternion rotation;

                    Transform::decomposeMatrix(matrix, position, rotation, scale);
                    return { position, rotation, scale };
                }
            );
        }

        transformType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<Transform>(typeName);
        return (void)typeInfo;
    }
}
