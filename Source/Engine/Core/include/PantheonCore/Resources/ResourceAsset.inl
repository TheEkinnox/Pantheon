#pragma once
#include <utility>

#include "ResourceAsset.h"

namespace PantheonEngine::Core::Resources
{
    inline ResourceAsset::ResourceAsset(std::string type, std::string guid, const std::string& path)
        : Asset(std::move(type), std::move(guid), path)
    {
    }

    inline bool ResourceAsset::isValid() const
    {
        const IResource* resource = GenericResourceRef(m_type, m_guid, m_path);
        return resource != nullptr;
    }

    inline bool ResourceAsset::getData(std::vector<char>& output) const
    {
        const IResource* resource = GenericResourceRef(m_type, m_guid, m_path);

        if (!resource)
        {
            DEBUG_LOG_ERROR("Unable to get resource data for asset \"%s\" (type: \"%s\" | path: \"%s\")",
                getGuid(), getType(), getPath());

            return false;
        }

        return resource->serialize(output);
    }
}
