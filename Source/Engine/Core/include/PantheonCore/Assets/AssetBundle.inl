#pragma once
#include "PantheonCore/Assets/AssetBundle.h"
#include "PantheonCore/Debug/Assertion.h"

namespace PantheonCore::Assets
{
    template <typename T>
    bool AssetBundle::add(const T& asset)
    {
        static_assert(std::is_base_of_v<Asset, T> || std::is_same_v<Asset, T>);

        if (!CHECK(!m_guidMap.contains(asset.getGuid()), "Unable to add asset to bundle - GUID \"%s\" is already used", asset.getGuid()))
            return false;

        if (!CHECK(!m_pathMap.contains(asset.getPath()), "Unable to add asset to bundle - Path \"%s\" is already used", asset.getPath()))
            return false;

        auto assetPtr = std::make_shared<T>(asset);

        if (!CHECK(assetPtr->isValid(), "Unable to add %s asset \"%s\" (path: \"%s\") to bundle - Invalid asset",
                asset.getType(), asset.getGuid(), asset.getPath()))
            return false;

        m_assets.emplace_back(assetPtr);
        m_guidMap[asset.getGuid()] = m_assets.size() - 1;
        m_pathMap[asset.getPath()] = m_assets.size() - 1;
        return true;
    }
}
