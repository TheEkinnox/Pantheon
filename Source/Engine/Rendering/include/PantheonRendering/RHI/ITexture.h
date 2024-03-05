#pragma once
#include "PantheonRendering/Enums/EPixelDataFormat.h"
#include "PantheonRendering/Enums/ETextureFilter.h"
#include "PantheonRendering/Enums/ETextureWrapMode.h"

#include <PantheonCore/Resources/IResource.h>

#include <string>

namespace PantheonRendering::RHI
{
    class ITexture : public PantheonCore::Resources::IResource
    {
        REGISTERED_RESOURCE_BODY(ITexture)

    public:
        /**
         * \brief Creates a copy of the given texture
         * \param other The texture to copy
         */
        ITexture(const ITexture& other);

        /**
         * \brief Creates a move copy of the given texture
         * \param other The texture to move
         */
        ITexture(ITexture&& other) noexcept;

        /**
         * \brief Releases resources allocated for the texture
         */
        ~ITexture() override;

        /**
         * \brief Assigns a copy of the given texture to this one
         * \param other The texture to copy
         * \return A reference to the modified texture
         */
        ITexture& operator=(const ITexture& other);

        /**
         * \brief Moves the given texture into this one
         * \param other The texture to move
         * \return A reference to the modified texture
         */
        ITexture& operator=(ITexture&& other) noexcept;

        /**
         * \brief Gets or creates a white 1x1 texture
         * \return A reference to the default texture
         */
        static ITexture& getDefault();

        /**
         * \brief Loads a texture from the given file
         * \param fileName The texture's file path
         * \return True if the texture was successfully loaded. False otherwise.
         */
        bool load(const std::string& fileName) override;

        /**
         * \brief Serializes the texture to a byte array
         * \param output The output memory buffer
         * \return True on success. False otherwise.
         */
        bool serialize(std::vector<char>& output) const override;

        /**
         * \brief Deserializes the texture from the given memory buffer
         * \param data A pointer to the beginning of the memory buffer
         * \param length The memory buffer's length
         * \return The number of deserialized bytes on success. 0 otherwise.
         */
        size_t deserialize(const void* data, size_t length) override;

        /**
         * \brief Binds the texture to the current context
         * \param slot The slot the texture should be bound to
         */
        virtual void bind(uint8_t slot) = 0;

        /**
         * \brief Unbinds the current texture from the current context
         * \param slot The slot the texture is bound to
         */
        virtual void unbind(uint8_t slot) = 0;

        /**
         * \brief Generates mipmaps for the texture
         */
        virtual void generateMipmap() = 0;

        /**
         * \brief Gets the textures dimensions
         * \return The texture's dimensions
         */
        LibMath::Vector2I getSize() const;

        /**
         * \brief Gets the texture's number of channels
         * \return The texture's channel count
         */
        uint8_t GetChannelCount() const;

        /**
         * \brief Sets the texture's horizontal wrap mode
         * \param wrapMode The texture's new horizontal wrap mode
         */
        void setWrapModeU(Enums::ETextureWrapMode wrapMode);

        /**
         * \brief Sets the texture's vertical wrap mode
         * \param wrapMode The texture's new vertical wrap mode
         */
        void setWrapModeV(Enums::ETextureWrapMode wrapMode);

        /**
         * \brief Set the texture's minification filter
         * \param filter The texture's new minification filter
         */
        void setMinFilter(Enums::ETextureFilter filter);

        /**
         * \brief Set the texture's magnification filter
         * \param filter The texture's new magnification filter
         */
        void setMagFilter(Enums::ETextureFilter filter);

        /**
         * \brief Gets the number of channels in the given pixel data format
         * \param format The target pixel data format
         * \return The number of channels in the given format
         */
        static uint8_t toChannelCount(Enums::EPixelDataFormat format);

        /**
         * \brief Creates a texture with the given width, height and pixel format
         * \param width The texture's width
         * \param height The texture's height
         * \param format The texture's pixel format
         */
        static std::shared_ptr<ITexture> create(int width, int height, Enums::EPixelDataFormat format);

    protected:
        unsigned char*          m_data      = nullptr;
        int                     m_width     = 0;
        int                     m_height    = 0;
        uint8_t                 m_channels  = 0;
        Enums::ETextureFilter   m_minFilter = Enums::ETextureFilter::LINEAR;
        Enums::ETextureFilter   m_magFilter = Enums::ETextureFilter::LINEAR;
        Enums::ETextureWrapMode m_wrapModeU = Enums::ETextureWrapMode::REPEAT;
        Enums::ETextureWrapMode m_wrapModeV = Enums::ETextureWrapMode::REPEAT;

        /**
         * \brief Creates a default texture
         */
        ITexture() = default;

        /**
         * \brief Creates a texture with the given width, height and pixel format
         * \param width The texture's width
         * \param height The texture's height
         * \param format The texture's pixel format
         */
        ITexture(int width, int height, Enums::EPixelDataFormat format);
    };
}

namespace PantheonCore::Resources
{
    template <>
    PantheonRendering::RHI::ITexture* createResource<PantheonRendering::RHI::ITexture>();

    template <>
    PantheonRendering::RHI::ITexture* getDefaultResource<PantheonRendering::RHI::ITexture>();
}
