#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include "PantheonScripting/LuaTypeRegistry.h"

#include <sol/state.hpp>

using namespace LibMath;

namespace PantheonScripting::Bindings
{
    void LuaMathBinder::bind(sol::state& luaState)
    {
        bindAngles(luaState);
        bindRotationOrder(luaState);
        bindMatrix(luaState);
        bindVector2(luaState);
        bindVector3(luaState);
        bindVector4(luaState);
        bindQuaternion(luaState);
        bindTransform(luaState);
        bindGlobals(luaState);
    }

    void LuaMathBinder::bindRotationOrder(sol::state& luaState)
    {
        static constexpr const char* typeName = "ERotationOrder";

        luaState.new_enum(typeName,
            "XYZ", ERotationOrder::XYZ,
            "XZY", ERotationOrder::XZY,
            "YXZ", ERotationOrder::YXZ,
            "YZX", ERotationOrder::YZX,
            "ZXY", ERotationOrder::ZXY,
            "ZYX", ERotationOrder::ZYX
        );

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<ERotationOrder>(typeName);
        return (void)typeInfo;
    }

    void LuaMathBinder::bindGlobals(sol::state& luaState)
    {
        sol::table math = luaState.get<sol::table>("math");

        /* Arithmetic */
        math.set_function("round", &LibMath::round<double>);
        math.set_function("clamp", sol::overload(
            &LibMath::clamp<double>,
            &LibMath::clamp<Vector2>,
            &LibMath::clamp<Vector3>,
            &LibMath::clamp<Vector4>
        ));

        math.set_function("snap", sol::overload(
            &LibMath::snap<double>,
            &LibMath::snap<Vector2>,
            &LibMath::snap<Vector3>,
            &LibMath::snap<Vector4>
        ));

        math.set_function("min", sol::overload(
            &LibMath::min<double>,
            &LibMath::min<Vector2>,
            &LibMath::min<Vector3>,
            &LibMath::min<Vector4>
        ));

        math.set_function("max", sol::overload(
            &LibMath::max<double>,
            &LibMath::max<Vector2>,
            &LibMath::max<Vector3>,
            &LibMath::max<Vector4>
        ));

        math.set_function("wrap", &LibMath::wrap<double>);
        math.set_function("sign", &LibMath::sign<double>);

        math.set_function("checkRange", sol::overload(
            &LibMath::isInRange<double>,
            &LibMath::isInRange<Vector2>,
            &LibMath::isInRange<Vector3>,
            &LibMath::isInRange<Vector4>
        ));

        math.set_function("isNear",
            [](const float a, const float b)
            {
                return floatEquals(a, b);
            }
        );

        /* Interpolation */
        math.set_function("lerp", sol::overload(
            &LibMath::lerp<double, double>,
            &LibMath::lerp<Vector2, float>,
            &LibMath::lerp<Vector3, float>,
            &LibMath::lerp<Vector4, float>,
            &LibMath::lerp<Quaternion, float>
        ));

        math.set_function("slerp", sol::overload(
            &LibMath::slerp<Vector2, float>,
            &LibMath::slerp<Vector3, float>,
            &LibMath::slerp<Vector4, float>,
            &LibMath::slerp<Quaternion, float>
        ));

        /* Trigonometry */
        math.set_function("sin", sol::overload(
            static_cast<double(*)(double)>(&std::sin),
            &LibMath::sin,
            [](const Degree& angle)
            {
                return LibMath::sin(angle);
            }
        ));

        math.set_function("cos", sol::overload(
            static_cast<double(*)(double)>(&std::cos),
            &LibMath::cos,
            [](const Degree& angle)
            {
                return LibMath::cos(angle);
            }
        ));

        math.set_function("tan", sol::overload(
            static_cast<double(*)(double)>(&std::tan),
            &LibMath::tan,
            [](const Degree& angle)
            {
                return LibMath::tan(angle);
            }
        ));

        math.set_function("asin", &LibMath::asin);
        math.set_function("acos", &LibMath::acos);
        math.set_function("atan", sol::overload(
            sol::resolve<Radian(float)>(&LibMath::atan),
            sol::resolve<Radian(float, float)>(&LibMath::atan)
        ));
    }
}
