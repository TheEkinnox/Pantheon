#pragma once

namespace PantheonCore::ECS
{
    class EntityHandle;

    struct ComponentTraits
    {
        /**
         * \brief The action to perform when a component of the given type is added
         * \tparam T The added entity's type
         * \param entity The component's owner
         * \param component The added component
         */
        template <class T>
        static void onAdd([[maybe_unused]] EntityHandle entity, [[maybe_unused]] T& component)
        {
        }

        /**
         * \brief The action to perform before a component of the given type is removed
         * \tparam T The removed component's type
         * \param entity The component's owner
         * \param component The component to remove
         */
        template <class T>
        static void onRemove([[maybe_unused]] EntityHandle entity, [[maybe_unused]] T& component)
        {
        }

        /**
         * \brief The action to perform before a component of the given type is updated
         * \tparam T The updated component's type
         * \param entity The component's owner
         * \param component The updated component
         */
        template <class T>
        static void onBeforeChange([[maybe_unused]] EntityHandle entity, [[maybe_unused]] T& component)
        {
        }

        /**
         * \brief The action to perform after a component of the given type was updated
         * \tparam T The updated component's type
         * \param entity The component's owner
         * \param component The updated component
         */
        template <class T>
        static void onChange([[maybe_unused]] EntityHandle entity, [[maybe_unused]] T& component)
        {
        }
    };
}

#include "PantheonCore/ECS/EntityHandle.h"
