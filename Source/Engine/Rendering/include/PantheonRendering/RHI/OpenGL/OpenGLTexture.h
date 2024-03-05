#pragma once
#include "PantheonRendering/RHI/ITexture.h"

namespace PantheonRendering::RHI
{
    class OpenGLTexture final : public ITexture
    {
    public:
        /**
         * \brief Creates a default texture
         */
        OpenGLTexture() = default;

        /**
         * \brief Creates a texture with the given width, height and pixel format
         * \param width The texture's width
         * \param height The texture's height
         * \param format The texture's pixel format
         */
        OpenGLTexture(int width, int height, Enums::EPixelDataFormat format);

        /**
         * \brief Creates a copy of the given texture
         * \param other The texture to copy
         */
        OpenGLTexture(const OpenGLTexture& other);

        /**
         * \brief Creates a move copy of the given texture
         * \param other The texture to move
         */
        OpenGLTexture(OpenGLTexture&& other) noexcept;

        /**
         * \brief Releases resources allocated for the texture
         */
        ~OpenGLTexture() override;

        /**
         * \brief Assigns a copy of the given texture to this one
         * \param other The texture to copy
         * \return A reference to the modified texture
         */
        OpenGLTexture& operator=(const OpenGLTexture& other);

        /**
         * \brief Moves the given texture into this one
         * \param other The texture to move
         * \return A reference to the modified texture
         */
        OpenGLTexture& operator=(OpenGLTexture&& other) noexcept;

        /**
         * \brief Gets or creates a white 1x1 texture
         * \return A reference to the default texture
         */
        static OpenGLTexture& getDefault();

        /**
         * \brief Initializes the texture
         * \return True if the texture was successfully initialized. False otherwise.
         */
        bool init() override;

        /**
         * \brief Binds the texture to the current OpenGL context
         * \param slot The slot the texture should be bound to
         */
        void bind(uint8_t slot) override;

        /**
         * \brief Unbinds the current texture from the OpenGL context
         * \param slot The slot the texture is bound to
         */
        void unbind(uint8_t slot) override;

        /**
         * \brief Generates mipmaps for the texture
         */
        void generateMipmap() override;

        /**
         * \brief Gets the texture's id
         * \return The texture's id
         */
        uint32_t getId() const;

    private:
        uint32_t m_id = 0;
    };
}
