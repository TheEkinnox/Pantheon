#pragma once
#include <PantheonCore/Debug/Assertion.h>

#include <cstdint>

namespace PantheonRendering::Enums
{
    /**
     * \brief Supported light types
     */
    enum class ELightType : uint8_t
    {
        AMBIENT,
        DIRECTIONAL,
        POINT,
        SPOT
    };

    /**
     * \brief Gets the given light type's string representation
     * \param type The light type to stringify
     * \return The given type's string representation
     */
    inline std::string lightTypeToString(const ELightType type)
    {
        switch (type)
        {
        case ELightType::AMBIENT:
            return "ambient";
        case ELightType::DIRECTIONAL:
            return "directional";
        case ELightType::POINT:
            return "point";
        case ELightType::SPOT:
            return "spot";
        default:
            PTH_ASSERT(false, "Unsupported light type");
            return {};
        }
    }

    /**
     * \brief Gets the light type represented by the given string
     * \param type The type string to convert
     * \return The type corresponding to the given string
     */
    inline ELightType stringToLightType(std::string type)
    {
        PantheonCore::Utility::toLowerInPlace(type);

        if (type == "ambient")
            return ELightType::AMBIENT;

        if (type == "directional")
            return ELightType::DIRECTIONAL;

        if (type == "point")
            return ELightType::POINT;

        if (type == "spot")
            return ELightType::SPOT;

        PTH_ASSERT(false, "Unsupported light type");
        return ELightType::AMBIENT;
    }
}
