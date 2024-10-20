
// This is a class pointing to a texture, exposing texture parameters for rendering

#pragma once

namespace GaladHen
{
    enum class TextureFormat;

    class Texture
    {

    public:

        Texture();

        Texture(unsigned char* textureBytes, int width, int height, int numberOfChannels, TextureFormat textureFormat, unsigned int numberOfMipMaps);

        // Texture copy handled explicitly by CreateTextureCopy()
        Texture(const Texture& sourceTexture) = delete;
        Texture& operator=(Texture& sourceTexture) = delete;

        Texture(Texture&& sourceTexture) noexcept;
        Texture& operator=(Texture&& sourceTexture) noexcept;


        const unsigned char* GetTextureData();
        int GetTextureWidth();
        int GetTextureHeight();
        int GetNumberOfChannels();
        TextureFormat GetTextureFormat();

        // @brief
        // Create a texture with a new allocation for texture bytes, then copying pixel values inside new texture
        Texture CreateTextureCopy();

        // @brief
        // Free texture resources
        void FreeResources();

        // @brief
        // Free texture resources
        ~Texture();

        unsigned int NumberOfMipMaps;

        unsigned int TextureID; // 0 means -1, so the low level pair is not already been created

    protected:

        unsigned char* TextureBytes;
        int Width, Height, NumberOfChannels;

        TextureFormat TexFormat;

    };
}
