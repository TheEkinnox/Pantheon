#include "PantheonRendering/Resources/TextureMetaData.h"

#include "PantheonCore/Debug/Assertion.h"
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Utility/TypeTraits.h"

using namespace PantheonCore::Serialization;
using namespace PantheonCore::Utility;
using namespace PantheonRendering::Enums;

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
        const HeaderT textureInfo = static_cast<HeaderT>(
            (static_cast<HeaderT>(m_minFilter) << MIN_FILTER_OFFSET) |
            (static_cast<HeaderT>(m_magFilter) << MAG_FILTER_OFFSET) |
            (static_cast<HeaderT>(m_wrapModeU) << WRAP_U_OFFSET) |
            (static_cast<HeaderT>(m_wrapModeV) << WRAP_V_OFFSET) |
            (static_cast<HeaderT>(m_generateMipmap) << GEN_MIP_OFFSET)
        );

        return CHECK(IByteSerializable::writeNumber(textureInfo, out), "Unable to serialize texture - Failed to write load info");
    }

    size_t TextureMetaData::fromBinary(const char* data, const size_t length)
    {
        HeaderT textureInfo;

        const size_t readBytes = IByteSerializable::readNumber(textureInfo, data, length);

        if (!CHECK(readBytes > 0, "Failed to read texture meta data"))
            return 0;

        m_minFilter = static_cast<ETextureFilter>(readBits(textureInfo, FILTER_BITS, MIN_FILTER_OFFSET));
        m_magFilter = static_cast<ETextureFilter>(readBits(textureInfo, FILTER_BITS, MAG_FILTER_OFFSET));

        m_wrapModeU = static_cast<ETextureWrapMode>(readBits(textureInfo, WRAP_MODE_BITS, WRAP_U_OFFSET));
        m_wrapModeV = static_cast<ETextureWrapMode>(readBits(textureInfo, WRAP_MODE_BITS, WRAP_V_OFFSET));

        m_generateMipmap = static_cast<bool>(readBits(textureInfo, BOOL_BITS, GEN_MIP_OFFSET));

        return readBytes;
    }

    bool TextureMetaData::toJson(JsonWriter& writer) const
    {
        writer.StartObject();

        writer.Key("wrap_u");
        if (!CHECK(writer.Uint(static_cast<uint8_t>(m_wrapModeU)), "Failed to write texture horizontal wrap mode"))
            return false;

        writer.Key("wrap_v");
        if (!CHECK(writer.Uint(static_cast<uint8_t>(m_wrapModeV)), "Failed to write texture vertical wrap mode"))
            return false;

        writer.Key("min_filter");
        if (!CHECK(writer.Uint(static_cast<uint8_t>(m_minFilter)), "Failed to write texture minification filter"))
            return false;

        writer.Key("mag_filter");
        if (!CHECK(writer.Uint(static_cast<uint8_t>(m_magFilter)), "Failed to write texture magnification filter"))
            return false;

        writer.Key("gen_mip");
        if (!CHECK(writer.Bool(m_generateMipmap), "Failed to write texture magnification filter"))
            return false;

        return CHECK(writer.EndObject(), "Failed to write texture meta data");
    }

    bool TextureMetaData::fromJson(const JsonValue& json)
    {
        if (!CHECK(json.IsObject(), "Unable to deserialize texture meta data - Json value should be an object"))
            return false;

        auto it = json.FindMember("wrap_u");
        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize texture horizontal wrap mode"))
            return false;

        m_wrapModeU = static_cast<ETextureWrapMode>(it->value.GetUint());

        it = json.FindMember("wrap_v");
        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize texture vertical wrap mode"))
            return false;

        m_wrapModeV = static_cast<ETextureWrapMode>(it->value.GetUint());

        it = json.FindMember("min_filter");
        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize texture minification filter"))
            return false;

        m_minFilter = static_cast<ETextureFilter>(it->value.GetUint());

        it = json.FindMember("mag_filter");
        if (!CHECK(it != json.MemberEnd() && it->value.IsUint(), "Unable to deserialize texture magnification filter"))
            return false;

        m_magFilter = static_cast<ETextureFilter>(it->value.GetUint());

        it = json.FindMember("gen_mip");
        if (!CHECK(it != json.MemberEnd() && it->value.IsBool(), "Unable to deserialize texture mipmap generation"))
            return false;

        m_generateMipmap = it->value.GetBool();

        return true;
    }
}
