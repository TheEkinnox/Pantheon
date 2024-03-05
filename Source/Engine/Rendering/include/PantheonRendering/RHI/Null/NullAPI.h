#pragma once
#include "PantheonRendering/RHI/IRenderAPI.h"

namespace PantheonRendering::RHI
{
    class NullAPI final : public IRenderAPI
    {
    public:
        ~NullAPI() override = default;

        NullAPI& init(bool enableDebug) override;

        NullAPI& clear(bool clearColor, bool clearDepth, bool clearStencil) override;

        NullAPI& readPixels(const PosT&           position, const DimensionsT& size, Enums::EPixelDataFormat dataFormat,
                            Enums::EPixelDataType dataType, void*              out) override;

        NullAPI& drawElements(Enums::EPrimitiveType primitiveType, uint32_t indexCount) override;
        NullAPI& drawElementsInstanced(Enums::EPrimitiveType primitiveType, uint32_t indexCount, uint32_t instances) override;

        NullAPI& drawArrays(Enums::EPrimitiveType primitiveType, uint32_t vertexCount) override;
        NullAPI& drawArraysInstanced(Enums::EPrimitiveType primitiveType, uint32_t vertexCount, uint32_t instances) override;

        NullAPI& setCapability(Enums::ERenderingCapability capability, bool enable) override;
        bool     hasCapability(Enums::ERenderingCapability capability) override;

        NullAPI& setDepthAlgorithm(Enums::ECompareAlgorithm algorithm) override;
        NullAPI& setStencilAlgorithm(Enums::ECompareAlgorithm algorithm, int32_t reference, uint32_t mask) override;

        NullAPI&    setClearColor(const Core::Color& color) override;
        Core::Color getClearColor() override;

        NullAPI& setCullFace(Enums::ECullFace cullFace) override;

        NullAPI& setDepthWriting(bool enable) override;
        NullAPI& setColorWriting(bool enableRed, bool enableGreen, bool enableBlue, bool enableAlpha) override;

        NullAPI& setBlendFunc(Enums::EBlendFactor sourceFactor, Enums::EBlendFactor destinationFactor) override;

        NullAPI& setViewport(PosT position, DimensionsT size) override;

        EGraphicsAPI getBackend() override;

        std::string getVendor() override;
        std::string getHardware() override;
        std::string getVersion() override;
        std::string getShadingLanguageVersion() override;
    };
}
