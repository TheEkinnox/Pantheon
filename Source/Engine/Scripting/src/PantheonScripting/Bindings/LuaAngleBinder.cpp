#include "PantheonScripting/LuaTypeRegistry.h"
#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include <sol/state.hpp>

using namespace LibMath;

namespace PantheonScripting::Bindings
{
    template <typename T, typename U>
    void bindAngleType(const std::string& typeName, sol::state& luaState)
    {
        sol::usertype angleType = luaState.new_usertype<T>(
            typeName, sol::constructors<T(), T(float), T(const U&)>(),
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const T& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const T& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            sol::meta_function::less_than, sol::overload(
                sol::resolve<bool(const T&, const T&)>(&operator<),
                sol::resolve<bool(const T&, const U&)>(&operator<)
            ),
            sol::meta_function::less_than_or_equal_to, sol::overload(
                sol::resolve<bool(const T&, const T&)>(&operator<=),
                sol::resolve<bool(const T&, const U&)>(&operator<=)
            ),
            sol::meta_function::equal_to, sol::overload(
                sol::resolve<bool(const T&, const T&)>(&operator==),
                sol::resolve<bool(const T&, const U&)>(&operator==)
            ),
            sol::meta_function::unary_minus, sol::resolve<T(const T&)>(&operator-),
            sol::meta_function::addition, sol::overload(
                sol::resolve<T(T, const T&)>(&operator+),
                [](const T& self, const U& other)
                {
                    return self + T(other);
                }
            ),
            sol::meta_function::subtraction, sol::overload(
                sol::resolve<T(T, const T&)>(&operator-),
                [](const T& self, const U& other)
                {
                    return self - T(other);
                }
            ),
            sol::meta_function::multiplication, sol::resolve<T(T, float)>(&operator*),
            sol::meta_function::division, sol::resolve<T(T, float)>(&operator/),
            "wrap", &T::wrap,
            "raw", sol::readonly_property(&T::raw)
        );

        angleType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<T>(typeName);
        return (void)typeInfo;
    }

    void LuaMathBinder::bindAngles(sol::state& luaState)
    {
        bindAngleType<Degree, Radian>("Degree", luaState);
        bindAngleType<Radian, Degree>("Radian", luaState);
    }
}
