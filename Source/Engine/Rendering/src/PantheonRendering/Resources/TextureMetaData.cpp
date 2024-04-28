#include "PantheonRendering/Resources/TextureMetaData.h"

#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/TypeTraits.h"

using namespace PantheonCore::Serialization;
using namespace PantheonCore::Utility;

namespace PantheonRendering::Resources
{
    static constexpr uint8_t MIN_FILTER_OFFSET = 0;
    static constexpr uint8_t MAG_FILTER_OFFSET = MIN_FILTER_OFFSET + TextureMetaData::FILTER_BITS;
    static constexpr uint8_t WRAP_U_OFFSET     = MAG_FILTER_OFFSET + TextureMetaData::FILTER_BITS;
    static constexpr uint8_t WRAP_V_OFFSET     = WRAP_U_OFFSET + TextureMetaData::WRAP_MODE_BITS;
    static constexpr uint8_t GEN_MIP_OFFSET    = WRAP_V_OFFSET + TextureMetaData::WRAP_MODE_BITS;

    using HeaderT = SmallestUInt<GEN_MIP_OFFSET + TextureMetaData::BOOL_BITS>;

    bool TextureMetaData::toBinary(std::vector<char>& out) const
    {
        const HeaderT textureInfo = (static_cast<HeaderT>(m_minFilter) << MIN_FILTER_OFFSET)
            | (static_cast<HeaderT>(m_magFilter) << MAG_FILTER_OFFSET)
            | (static_cast<HeaderT>(m_wrapModeU) << WRAP_U_OFFSET)
            | (static_cast<HeaderT>(m_wrapModeV) << WRAP_V_OFFSET)
            | (static_cast<HeaderT>(m_generateMipmap) << GEN_MIP_OFFSET);

        return CHECK(IByteSerializable::writeNumber(textureInfo, out), "Unable to serialize texture - Failed to write load info");
    }

    size_t TextureMetaData::fromBinary(const char* data, size_t length)
    {
        HeaderT textureInfo;

        const size_t readBytes = IByteSerializable::readNumber(textureInfo, data, length);

        if (!CHECK(readBytes > 0, "Failed to read texture meta data"))
            return 0;

        m_minFilter = static_cast<Enums::ETextureFilter>(readBits(textureInfo, FILTER_BITS, MIN_FILTER_OFFSET));
        m_magFilter = static_cast<Enums::ETextureFilter>(readBits(textureInfo, FILTER_BITS, MAG_FILTER_OFFSET));

        m_wrapModeU = static_cast<Enums::ETextureWrapMode>(readBits(textureInfo, WRAP_MODE_BITS, WRAP_U_OFFSET));
        m_wrapModeV = static_cast<Enums::ETextureWrapMode>(readBits(textureInfo, WRAP_MODE_BITS, WRAP_V_OFFSET));

        m_generateMipmap = static_cast<bool>(readBits(textureInfo, BOOL_BITS, GEN_MIP_OFFSET));

        return readBytes;
    }

    bool TextureMetaData::toJson(JsonWriter& /*writer*/) const
    {
        // TODO: Texture meta data to json
        return CHECK(false, "TODO: Texture meta data to json");
    }

    bool TextureMetaData::fromJson(const JsonValue& /*json*/)
    {
        // TODO: Texture meta data from json
        return CHECK(false, "TODO: Texture meta data from json");
    }
}
