#pragma once
#include "PantheonRendering/RHI/ITexture.h"

namespace PantheonRendering::RHI
{
    class NullTexture final : public ITexture
    {
    public:
        /**
         * \brief Creates a default texture
         */
        NullTexture() = default;

        /**
         * \brief Creates a texture with the given width, height and pixel format
         * \param width The texture's width
         * \param height The texture's height
         * \param format The texture's pixel format
         */
        NullTexture(int width, int height, Enums::EPixelDataFormat format);

        /**
         * \brief Creates a copy of the given texture
         * \param other The texture to copy
         */
        NullTexture(const NullTexture& other) = default;

        /**
         * \brief Creates a move copy of the given texture
         * \param other The texture to move
         */
        NullTexture(NullTexture&& other) noexcept = default;

        /**
         * \brief Releases resources allocated for the texture
         */
        ~NullTexture() override = default;

        /**
         * \brief Assigns a copy of the given texture to this one
         * \param other The texture to copy
         * \return A reference to the modified texture
         */
        NullTexture& operator=(const NullTexture& other) = default;

        /**
         * \brief Moves the given texture into this one
         * \param other The texture to move
         * \return A reference to the modified texture
         */
        NullTexture& operator=(NullTexture&& other) noexcept = default;

        /**
         * \brief Gets or creates a white 1x1 texture
         * \return A reference to the default texture
         */
        static NullTexture& getDefault();

        /**
         * \brief Initializes the texture
         * \return True if the texture was successfully initialized. False otherwise.
         */
        bool init() override;

        /**
         * \brief Binds the texture to the current context
         * \param slot The slot the texture is bound to
         */
        void bind(uint8_t slot) override;

        /**
         * \brief Unbinds the current texture from the context
         * \param slot The slot the texture is bound to
         */
        void unbind(uint8_t slot) override;

        /**
         * \brief Generates mipmaps for the texture
         */
        void generateMipmap() override;
    };
}
