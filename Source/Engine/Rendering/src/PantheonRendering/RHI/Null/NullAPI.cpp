#include "PantheonRendering/RHI/Null/NullAPI.h"

#include <PantheonCore/Debug/Assertion.h>

namespace PantheonRendering::RHI
{
    NullAPI& NullAPI::init(bool)
    {
        return *this;
    }

    NullAPI& NullAPI::clear(bool, bool, bool)
    {
        return *this;
    }

    NullAPI& NullAPI::readPixels(const PosT&, const DimensionsT&, Enums::EPixelDataFormat, Enums::EPixelDataType, void*)
    {
        return *this;
    }

    NullAPI& NullAPI::drawElements(Enums::EPrimitiveType, uint32_t)
    {
        return *this;
    }

    NullAPI& NullAPI::drawElementsInstanced(Enums::EPrimitiveType, uint32_t, uint32_t)
    {
        return *this;
    }

    NullAPI& NullAPI::drawArrays(Enums::EPrimitiveType, uint32_t)
    {
        return *this;
    }

    NullAPI& NullAPI::drawArraysInstanced(Enums::EPrimitiveType, uint32_t, uint32_t)
    {
        return *this;
    }

    NullAPI& NullAPI::setCapability(Enums::ERenderingCapability, bool)
    {
        return *this;
    }

    bool NullAPI::hasCapability(Enums::ERenderingCapability)
    {
        return false;
    }

    NullAPI& NullAPI::setDepthAlgorithm(Enums::ECompareAlgorithm)
    {
        return *this;
    }

    NullAPI& NullAPI::setStencilAlgorithm(Enums::ECompareAlgorithm, int32_t, uint32_t)
    {
        return *this;
    }

    NullAPI& NullAPI::setClearColor(const Core::Color&)
    {
        return *this;
    }

    Core::Color NullAPI::getClearColor()
    {
        return Core::Color::black;
    }

    NullAPI& NullAPI::setCullFace(Enums::ECullFace)
    {
        return *this;
    }

    NullAPI& NullAPI::setDepthWriting(bool)
    {
        return *this;
    }

    NullAPI& NullAPI::setColorWriting(bool, bool, bool, bool)
    {
        return *this;
    }

    NullAPI& NullAPI::setBlendFunc(Enums::EBlendFactor, Enums::EBlendFactor)
    {
        return *this;
    }

    NullAPI& NullAPI::setViewport(PosT, DimensionsT)
    {
        return *this;
    }

    EGraphicsAPI NullAPI::getBackend()
    {
        return EGraphicsAPI::NONE;
    }

    std::string NullAPI::getVendor()
    {
        return "None";
    }

    std::string NullAPI::getHardware()
    {
        return "None";
    }

    std::string NullAPI::getVersion()
    {
        return "None";
    }

    std::string NullAPI::getShadingLanguageVersion()
    {
        return "None";
    }
}
