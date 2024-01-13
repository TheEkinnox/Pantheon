#pragma once
#include "ResourceRef.h"

#include "PantheonCore/Assets/Asset.h"

namespace PantheonCore::Resources
{
    class ResourceAsset final : public Assets::Asset
    {
    public:
        ResourceAsset() = default;

        /**
         * \brief Creates an asset with the given values
         * \param type The resource's type
         * \param guid The resource's guid
         * \param path The resource's path
         */
        ResourceAsset(std::string type, std::string guid, const std::string& path);

        /**
         * \brief Checks whether the resource is valid or not
         * \return True if the resource is valid. False otherwise
         */
        bool isValid() const override;

        using Asset::getData;

        /**
         * \brief Writes the asset's data in a memory buffer
         * \param output The target memory buffer
         * \return True if the data could be read. False otherwise
         */
        inline bool getData(std::vector<char>& output) const override;
    };
}

#include "PantheonCore/Resources/ResourceAsset.inl"
