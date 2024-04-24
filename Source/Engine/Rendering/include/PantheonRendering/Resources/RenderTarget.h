#pragma once
#include "TextureMetaData.h"

#include "PantheonRendering/Enums/EPixelDataFormat.h"
#include "PantheonRendering/Enums/ETextureFilter.h"
#include "PantheonRendering/Enums/ETextureWrapMode.h"
#include "PantheonRendering/RHI/IFrameBuffer.h"
#include "PantheonRendering/RHI/ITexture.h"

#include <PantheonCore/Resources/IResource.h>

namespace PantheonRendering::Enums
{
    enum class ETextureWrapMode : uint8_t;
}

namespace PantheonRendering::Resources
{
    class RenderTarget final : public PantheonCore::Resources::IResource
    {
        REGISTERED_RESOURCE_BODY()

    public:
        /**
         * \brief Loads the render target from the given file
         * \param fileName The path of the render target to load
         * \return True if the render target was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        /**
         * \brief Initializes the render target
         * \return True on success. False otherwise
         */
        bool init() override;

        /**
         * \brief Saves the render target to the given file
         * \param fileName The target save path
         * \return True if the render target was successfully saved. False otherwise.
         */
        bool save(const std::string& fileName) const override;

        /**
         * \brief Serializes the render target to json
         * \param writer The output json writer
         * \return True on success. False otherwise.
         */
        bool toJson(PantheonCore::Serialization::JsonWriter& writer) const;

        /**
         * \brief Deserializes the render target from json
         * \param json The input json data
         * \return True on success. False otherwise.
         */
        bool fromJson(const PantheonCore::Serialization::JsonValue& json);

        /**
         * \brief Serializes the render target to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool toBinary(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the render target from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t fromBinary(const char* data, size_t length) override;

        RHI::IFrameBuffer&           getFrameBuffer() const;
        std::weak_ptr<RHI::ITexture> getColorTexture() const;
        std::weak_ptr<RHI::ITexture> getDepthTexture() const;

    private:
        std::unique_ptr<RHI::IFrameBuffer> m_frameBuffer;
        std::shared_ptr<RHI::ITexture>     m_colorTexture;
        std::shared_ptr<RHI::ITexture>     m_depthTexture;

        int                     m_width       = 0;
        int                     m_height      = 0;
        Enums::EPixelDataFormat m_colorFormat = Enums::EPixelDataFormat::RGB;
        Enums::EPixelDataFormat m_depthFormat = Enums::EPixelDataFormat::DEPTH_STENCIL;
        TextureMetaData         m_loadInfo;
    };
}
