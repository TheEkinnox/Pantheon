#pragma once
#include <PantheonCore/Debug/Assertion.h>

#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported camera projection types
     */
    enum class EProjectionType : uint8_t
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    /**
     * \brief Gets the given projection type's string representation
     * \param type The projection type to stringify
     * \return The given type's string representation
     */
    inline std::string projectionTypeToString(const EProjectionType type)
    {
        switch (type)
        {
        case EProjectionType::PERSPECTIVE:
            return "perspective";
        case EProjectionType::ORTHOGRAPHIC:
            return "orthographic";
        default:
            PTH_ASSERT(false, "Unsupported projection type");
            return {};
        }
    }

    /**
     * \brief Gets the projection type represented by the given string
     * \param type The type string to convert
     * \return The type corresponding to the given string
     */
    inline EProjectionType stringToProjectionType(std::string type)
    {
        PantheonCore::Utility::toLowerInPlace(type);

        if (type == "perspective")
            return EProjectionType::PERSPECTIVE;

        if (type == "orthographic")
            return EProjectionType::ORTHOGRAPHIC;

        PTH_ASSERT(false, "Unsupported projection type");
        return EProjectionType::PERSPECTIVE;
    }
}
