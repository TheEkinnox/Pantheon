#include "PantheonScripting/LuaTypeRegistry.h"
#include "PantheonScripting/Bindings/LuaMathBinder.h"

#include <Matrix.h>

using namespace LibMath;

namespace PantheonScripting::Bindings
{
    template <length_t Rows, length_t Cols, typename DataT>
    void bindMatrixType(const std::string& typeName, sol::state& luaState)
    {
        using MatT = TMatrix<Rows, Cols, DataT>;

        sol::constructors<
            MatT(),
            MatT(DataT),
            MatT(const MatT&)
        > ctor;

        sol::usertype matType = luaState.new_usertype<MatT>(
            typeName, ctor,
            sol::meta_function::concatenation, sol::overload(
                [](const char* str, const MatT& self)
                {
                    std::ostringstream oss;
                    oss << str << self;
                    return oss.str();
                },
                [](const MatT& self, const char* str)
                {
                    std::ostringstream oss;
                    oss << self << str;
                    return oss.str();
                }
            ),
            sol::meta_function::unary_minus, [](const MatT& self)
            {
                return -self;
            },
            sol::meta_function::multiplication, sol::overload(
                [](const MatT& self, const DataT scalar)
                {
                    return self * scalar;
                },
                [](const MatT& self, const MatT& other)
                {
                    return self * other;
                }
            ),
            sol::meta_function::division, sol::overload(
                [](const MatT& self, const DataT scalar)
                {
                    return self / scalar;
                },
                [](const MatT& self, const MatT& other)
                {
                    return self / other;
                }
            ),
            sol::meta_function::call, sol::resolve<DataT(length_t, length_t) const>(&MatT::operator()),
            "rowCount", sol::readonly_property(&MatT::getRowCount),
            "columnCount", sol::readonly_property(&MatT::getColumnCount),
            "size", sol::readonly_property(&MatT::getSize),
            "identity", sol::readonly_property([]
            {
                return MatT(DataT(1));
            }),
            "zero", sol::readonly_property([]
            {
                return MatT();
            }),
            "get", sol::resolve<DataT(length_t, length_t) const>(&MatT::operator()),
            "set", [](MatT& self, const length_t row, const length_t col, const DataT value)
            {
                self(row, col) = value;
            },
            "isIdentity", sol::readonly_property(&MatT::isIdentity),
            "determinant", sol::readonly_property(&MatT::determinant),
            "cofactor", &MatT::cofactor,
            "minor", &MatT::minor,
            "transposed", sol::readonly_property(&MatT::transposed),
            "coMatrix", sol::readonly_property(&MatT::coMatrix),
            "adjugate", sol::readonly_property(&MatT::adjugate),
            "inverse", sol::readonly_property(&MatT::inverse),
            "getIndex", &MatT::getIndex
        );

        matType["__type"]["name"] = typeName;

        static const LuaTypeInfo& typeInfo = LuaTypeRegistry::getInstance().registerType<MatT>(typeName);
        return (void)typeInfo;
    }

    void LuaMathBinder::bindMatrix(sol::state& luaState)
    {
        bindMatrixType<3, 3, float>("Matrix3", luaState);

        static constexpr const char* mat4Name = "Matrix4";
        bindMatrixType<4, 4, float>(mat4Name, luaState);

        {
            sol::table meta = luaState.get<sol::table>(mat4Name);
            meta.set_function("translation", sol::overload(
                    sol::resolve<Matrix4(float, float, float)>(&translation),
                    sol::resolve<Matrix4(const Vector3&)>(&translation)
                )
            );

            meta.set_function("scaling", sol::overload(
                    sol::resolve<Matrix4(float, float, float)>(&scaling),
                    sol::resolve<Matrix4(const Vector3&)>(&scaling)
                )
            );

            meta.set_function("rotation", sol::overload(
                    sol::resolve<Matrix4(const Quaternion&)>(&rotation),
                    sol::resolve<Matrix4(const Radian&, const Vector3&)>(&rotation),
                    [](const Degree& angle, const Vector3& axis)
                    {
                        return rotation(angle, axis);
                    }
                )
            );

            meta.set_function("rotationFromTo", &LibMath::rotationFromTo<float>);
            meta.set_function("orthographicProjection", &LibMath::orthographicProjection<float>);
            meta.set_function("perspectiveProjection", &LibMath::perspectiveProjection<float>);
            meta.set_function("lookAt", &LibMath::lookAt<float>);
            meta.set_function("toEuler",
                [](const Matrix4& mat, const ERotationOrder order)
                -> std::tuple<Radian, Radian, Radian>
                {
                    TVector3<Radian> angles = toEuler(mat, order);
                    return { angles.m_x, angles.m_y, angles.m_z };
                }
            );
        }
    }
}
