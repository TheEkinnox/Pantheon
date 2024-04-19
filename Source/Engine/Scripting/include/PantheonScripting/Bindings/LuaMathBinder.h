#pragma once
#include <PantheonCore/ECS/Serializers/MathSerializers.h>

namespace sol
{
    class state;
}

namespace PantheonScripting::Bindings
{
    class LuaMathBinder final
    {
    public:
        /**
         * \brief Creates or updates the math bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bind(sol::state& luaState);

    private:
        /**
         * \brief Creates or updates the angle bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindAngles(sol::state& luaState);

        /**
         * \brief Creates or updates the rotation order bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindRotationOrder(sol::state& luaState);

        /**
         * \brief Creates or updates the matrix bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindMatrix(sol::state& luaState);

        /**
         * \brief Creates or updates the 2D vector bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindVector2(sol::state& luaState);

        /**
         * \brief Creates or updates the 3D vector bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindVector3(sol::state& luaState);

        /**
         * \brief Creates or updates the 4D vector bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindVector4(sol::state& luaState);

        /**
         * \brief Creates or updates the quaternion bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindQuaternion(sol::state& luaState);

        /**
         * \brief Creates or updates the transform bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindTransform(sol::state& luaState);

        /**
         * \brief Creates or updates the global math bindings in the given lua state
         * \param luaState The target lua state
         */
        static void bindGlobals(sol::state& luaState);
    };
}
