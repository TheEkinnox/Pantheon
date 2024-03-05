#pragma once
#include "PantheonRendering/Enums/EAccessMode.h"
#include "PantheonRendering/RHI/IRenderAPI.h"

namespace PantheonRendering::RHI
{
    class OpenGLAPI final : public IRenderAPI
    {
    public:
        ~OpenGLAPI() override = default;

        OpenGLAPI& init(bool enableDebug) override;

        OpenGLAPI& clear(bool clearColor, bool clearDepth, bool clearStencil) override;

        OpenGLAPI& readPixels(const PosT&           position, const DimensionsT& size, Enums::EPixelDataFormat dataFormat,
                              Enums::EPixelDataType dataType, void*              out) override;

        OpenGLAPI& drawElements(Enums::EPrimitiveType primitiveType, uint32_t indexCount) override;
        OpenGLAPI& drawElementsInstanced(Enums::EPrimitiveType primitiveType, uint32_t indexCount, uint32_t instances) override;

        OpenGLAPI& drawArrays(Enums::EPrimitiveType primitiveType, uint32_t vertexCount) override;
        OpenGLAPI& drawArraysInstanced(Enums::EPrimitiveType primitiveType, uint32_t vertexCount, uint32_t instances) override;

        OpenGLAPI& setCapability(Enums::ERenderingCapability capability, bool enable) override;
        bool       hasCapability(Enums::ERenderingCapability capability) override;

        OpenGLAPI& setDepthAlgorithm(Enums::ECompareAlgorithm algorithm) override;
        OpenGLAPI& setStencilAlgorithm(Enums::ECompareAlgorithm algorithm, int32_t reference, uint32_t mask) override;

        OpenGLAPI&  setClearColor(const Core::Color& color) override;
        Core::Color getClearColor() override;

        OpenGLAPI& setCullFace(Enums::ECullFace cullFace) override;

        OpenGLAPI& setDepthWriting(bool enable) override;
        OpenGLAPI& setColorWriting(bool enableRed, bool enableGreen, bool enableBlue, bool enableAlpha) override;

        OpenGLAPI& setBlendFunc(Enums::EBlendFactor sourceFactor, Enums::EBlendFactor destinationFactor) override;

        OpenGLAPI& setViewport(PosT position, DimensionsT size) override;

        EGraphicsAPI getBackend() override;

        std::string getVendor() override;
        std::string getHardware() override;
        std::string getVersion() override;
        std::string getShadingLanguageVersion() override;

        static unsigned toGLEnum(Enums::EPixelDataFormat dataFormat);
        static unsigned toGLEnum(Enums::EPixelDataType dataType);
        static unsigned toGLEnum(Enums::EPrimitiveType primitiveType);
        static unsigned toGLEnum(Enums::ERenderingCapability capability);
        static unsigned toGLEnum(Enums::ECompareAlgorithm algorithm);
        static unsigned toGLEnum(Enums::ECullFace cullFace);
        static unsigned toGLEnum(Enums::EBlendFactor blendFactor);
        static unsigned toGLEnum(Enums::EAccessMode accessSpecifier);
    };
}
