#pragma once
#include "PantheonCore/Resources/ResourceAsset.h"
#include "PantheonCore/Resources/ResourceRef.h"
#include "PantheonCore/Utility/ServiceLocator.h"

#include <utility>

namespace PantheonCore::Resources
{
    inline ResourceAsset::ResourceAsset(std::string type, std::string guid, const std::string& path)
        : Asset(std::move(type), std::move(guid), path)
    {
    }

    inline bool ResourceAsset::isValid() const
    {
        return GenericResourceRef(m_type, m_guid, m_path).hasValue();
    }

    inline bool ResourceAsset::getData(std::vector<char>& output) const
    {
        const GenericResourceRef resource(m_type, m_guid, m_path);

        if (!CHECK(resource.hasValue(), "Unable to get resource data for asset \"%s\" (type: \"%s\" | path: \"%s\")",
                getGuid(), getType(), getPath()))
            return false;

        return resource->toBinary(output);
    }
}
