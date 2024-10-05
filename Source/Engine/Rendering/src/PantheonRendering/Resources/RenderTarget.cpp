#include "PantheonRendering/Resources/RenderTarget.h"

#include <rapidjson/istreamwrapper.h>

using namespace PantheonCore::Serialization;

namespace PantheonRendering::Resources
{
    bool RenderTarget::load(const std::string& fileName)
    {
        std::ifstream fs(fileName);

        if (!CHECK(fs.is_open(), "Unable to open render target file at path \"%s\"", fileName.c_str()))
            return false;

        rapidjson::IStreamWrapper isw(fs);

        rapidjson::Document json;
        json.ParseStream(isw);

        if (!CHECK(!json.HasParseError(), "Unable to parse render target from file - Parse error %d", json.GetParseError()))
            return false;

        return fromJson(json);
    }

    bool RenderTarget::init()
    {
        m_frameBuffer = RHI::IFrameBuffer::create();

        PTH_ASSERT(m_colorFormat >= Enums::EPixelDataFormat::RED || m_colorFormat == Enums::EPixelDataFormat::NONE);
        PTH_ASSERT(m_depthFormat < Enums::EPixelDataFormat::RED);

        if (m_colorFormat != Enums::EPixelDataFormat::NONE)
        {
            m_colorTexture = RHI::ITexture::create(m_width, m_height, m_colorFormat);
            m_colorTexture->setWrapModeU(m_loadInfo.m_wrapModeU);
            m_colorTexture->setWrapModeV(m_loadInfo.m_wrapModeV);

            if (m_loadInfo.m_generateMipmap)
                m_colorTexture->generateMipmap();

            m_frameBuffer->attach(*m_colorTexture, Enums::EFrameBufferAttachment::COLOR);
        }

        if (m_depthFormat != Enums::EPixelDataFormat::NONE)
        {
            Enums::EFrameBufferAttachment attachment;

            switch (m_depthFormat)
            {
            case Enums::EPixelDataFormat::STENCIL_INDEX:
                attachment = Enums::EFrameBufferAttachment::STENCIL;
                break;
            case Enums::EPixelDataFormat::DEPTH_COMPONENT:
                attachment = Enums::EFrameBufferAttachment::DEPTH;
                break;
            case Enums::EPixelDataFormat::DEPTH_STENCIL:
                attachment = Enums::EFrameBufferAttachment::DEPTH_STENCIL;
                break;
            default:
                PTH_ASSERT(false, "Invalid depth texture format");
                return false;
            }

            m_depthTexture = RHI::ITexture::create(m_width, m_height, m_depthFormat);
            m_depthTexture->setMinFilter(m_loadInfo.m_minFilter);
            m_depthTexture->setMagFilter(m_loadInfo.m_magFilter);
            m_depthTexture->setWrapModeU(m_loadInfo.m_wrapModeU);
            m_depthTexture->setWrapModeV(m_loadInfo.m_wrapModeV);

            if (m_loadInfo.m_generateMipmap)
                m_depthTexture->generateMipmap();

            m_frameBuffer->attach(*m_depthTexture, attachment);
        }

        return true;
    }

    bool RenderTarget::save(const std::string& fileName) const
    {
        rapidjson::StringBuffer buffer;
        JsonWriter              writer(buffer);

        if (!toJson(writer) || !ASSUME(writer.IsComplete(), "Failed to save render target - Generated json is incomplete"))
            return false;

        std::ofstream fs(fileName);

        if (!CHECK(fs.is_open(), "Unable to open render target file at path \"%s\"", fileName.c_str()))
            return false;

        fs << std::string_view(buffer.GetString(), buffer.GetLength());

        return CHECK(!fs.bad(), "Failed to write render target to \"%s\"", fileName.c_str());
    }

    bool RenderTarget::toJson(JsonWriter& writer) const
    {
        // TODO: render target to json
        return CHECK(false, "TODO: render target to json") && m_loadInfo.toJson(writer);
    }

    bool RenderTarget::fromJson(const JsonValue& /*json*/)
    {
        // TODO: render target from json
        return CHECK(false, "TODO: render target to json");
    }

    bool RenderTarget::toBinary(std::vector<char>& output) const
    {
        // TODO: render target to binary
        return CHECK(false, "TODO: render target to json") && m_loadInfo.toBinary(output);
    }

    size_t RenderTarget::fromBinary(const char* /*data*/, size_t /*length*/)
    {
        // TODO: render target from binary
        CHECK(false, "TODO: render target to json");
        return 0;
    }

    RHI::IFrameBuffer& RenderTarget::getFrameBuffer() const
    {
        PTH_ASSERT(m_frameBuffer, "Uninitialized render target");
        return *m_frameBuffer;
    }

    std::weak_ptr<RHI::ITexture> RenderTarget::getColorTexture() const
    {
        return m_colorTexture;
    }

    std::weak_ptr<RHI::ITexture> RenderTarget::getDepthTexture() const
    {
        return m_depthTexture;
    }
}
