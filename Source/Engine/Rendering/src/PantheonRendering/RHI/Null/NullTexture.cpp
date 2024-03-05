#include "PantheonRendering/RHI/Null/NullTexture.h"

namespace PantheonRendering::RHI
{
    NullTexture::NullTexture(const int width, const int height, const Enums::EPixelDataFormat format)
        : ITexture(width, height, format)
    {
    }

    NullTexture& NullTexture::getDefault()
    {
        static NullTexture texture;
        return texture;
    }

    bool NullTexture::init()
    {
        return true;
    }

    void NullTexture::bind(uint8_t)
    {
    }

    void NullTexture::unbind(uint8_t)
    {
    }

    void NullTexture::generateMipmap()
    {
    }
}
