#pragma once
#include <unordered_map>

#include "PantheonCore/Debug/Assertion.h"

#define PTH_SERVICE(Type) PantheonEngine::Core::Utility::ServiceLocator::get<Type>()

namespace PantheonEngine::Core::Utility
{
    class ServiceLocator
    {
    public:
        /**
         * \brief Provides a service of the given type to the service locator.
         * \tparam T The provided service's type
         * \param service The provided service
         */
        template <typename T>
        static void provide(T& service)
        {
            s_services[typeid(T).hash_code()] = &service;
        }

        /**
         * \brief Gets the service of the given type.
         * \tparam T The service's type
         * \return A reference to the previously provided service.
         */
        template <typename T>
        static T& get()
        {
            ASSERT(s_services.contains(typeid(T).hash_code()));
            return *static_cast<T*>(s_services[typeid(T).hash_code()]);
        }

    private:
        inline static std::unordered_map<size_t, void*> s_services;
    };
}
