#pragma once
#include "PantheonCore/Serialization/IByteSerializable.h"
#include "PantheonCore/Serialization/Serializer.h"

#include "PantheonRendering/Enums/ETextureFilter.h"
#include "PantheonRendering/Enums/ETextureWrapMode.h"

#include <vector>

namespace PantheonRendering::Resources
{
    struct TextureMetaData
    {
        static constexpr uint8_t WRAP_MODE_BITS = 4;
        static constexpr uint8_t FILTER_BITS    = 6;
        static constexpr uint8_t BOOL_BITS      = 1;

        Enums::ETextureWrapMode m_wrapModeU       : WRAP_MODE_BITS = Enums::ETextureWrapMode::REPEAT;
        Enums::ETextureWrapMode m_wrapModeV       : WRAP_MODE_BITS = Enums::ETextureWrapMode::REPEAT;
        Enums::ETextureFilter   m_minFilter       : FILTER_BITS    = Enums::ETextureFilter::LINEAR;
        Enums::ETextureFilter   m_magFilter       : FILTER_BITS    = Enums::ETextureFilter::LINEAR;
        bool                    m_generateMipmap : BOOL_BITS      = true;

        /**
         * \brief Serializes the texture meta data to a byte array
         * \param out The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& out) const;

        /**
         * \brief Deserializes the texture meta data from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length);

        /**
         * \brief Serializes the texture to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool toJson(PantheonCore::Serialization::JsonWriter& writer) const;

        /**
         * \brief Deserializes the texture from json
         * \param json The input json value
         * \return True on success. False otherwise.
         */
        bool fromJson(const PantheonCore::Serialization::JsonValue& json);
    };
}
