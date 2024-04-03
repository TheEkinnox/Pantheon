#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Resources/ResourceRegistry.h"

#include <string>

#define REGISTERED_RESOURCE_BODY()                                                                            \
public:                                                                                                       \
inline const std::string& getTypeName() const                                                                 \
{                                                                                                             \
    return PantheonCore::Resources::ResourceRegistry::getInstance().getRegisteredTypeName<decltype(*this)>(); \
}                                                                                                             \
private:

namespace PantheonCore::Resources
{
    class IResource : public Serialization::IByteSerializable
    {
    public:
        /**
         * \brief Destroys the resource
         */
        ~IResource() override = default;

        /**
         * \brief Gets the resource's registered type name
         * \return The resource's registered type name
         */
        virtual const std::string& getTypeName() const = 0;

        /**
         * \brief Tries to load the resource from the given file
         * \param fileName The resource file's path
         * \return True if the resource was successfully loaded. False otherwise.
         */
        virtual bool load(const std::string& fileName) = 0;

        /**
         * \brief Initializes the resource
         * \return True on success. False otherwise.
         */
        virtual bool init() = 0;
    };
}
