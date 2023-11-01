#pragma once
#include "PantheonCore/Assets/AssetBundle.h"
#include "PantheonCore/Debug/Assertion.h"

namespace PantheonEngine::Core::Assets
{
    template <typename T>
    bool AssetBundle::add(const T& asset)
    {
        static_assert(std::is_base_of_v<Asset, T> || std::is_same_v<Asset, T>);

        if (m_guidMap.contains(asset.getGuid()))
        {
            DEBUG_LOG_ERROR("Unable to add asset to bundle - GUID \"%s\" is already used", asset.getGuid());
            return false;
        }

        if (m_pathMap.contains(asset.getPath()))
        {
            DEBUG_LOG_ERROR("Unable to add asset to bundle - Path \"%s\" is already used", asset.getPath());
            return false;
        }

        auto assetPtr = std::make_shared<T>(asset);

        if (!assetPtr->isValid())
        {
            DEBUG_LOG_ERROR("Unable to add %s asset \"%s\" (path: \"%s\") to bundle - Invalid asset",
                asset.getType(), asset.getGuid(), asset.getPath());
            return false;
        }

        m_assets.emplace_back(assetPtr);
        m_guidMap[asset.getGuid()] = m_assets.size() - 1;
        m_pathMap[asset.getPath()] = m_assets.size() - 1;
        return true;
    }
}
