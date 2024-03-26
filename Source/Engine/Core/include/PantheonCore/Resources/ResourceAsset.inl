#pragma once
#include <utility>

#include "ResourceAsset.h"

namespace PantheonCore::Resources
{
    inline ResourceAsset::ResourceAsset(std::string type, std::string guid, const std::string& path)
        : Asset(std::move(type), std::move(guid), path)
    {
    }

    inline bool ResourceAsset::isValid() const
    {
        return *GenericResourceRef(m_type, m_guid, m_path) != nullptr;
    }

    inline bool ResourceAsset::getData(std::vector<char>& output) const
    {
        const IResource* resource = *GenericResourceRef(m_type, m_guid, m_path);

        if (!resource)
        {
            DEBUG_LOG_ERROR("Unable to get resource data for asset \"%s\" (type: \"%s\" | path: \"%s\")",
                getGuid(), getType(), getPath());

            return false;
        }

        return resource->toBinary(output);
    }
}
