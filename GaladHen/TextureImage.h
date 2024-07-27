
// This is the class holding texture actual data (pixel colors and alpha)

#pragma once

#include <string>

namespace GaladHen
{
    class TextureImage
    {

    public:

        TextureImage();

        // @brief
        // Create a texture image from image data
        TextureImage(unsigned char* textureBytes, int widht, int height, int channels);

        // TextureImage has not copy constructor and assignments -> a TextureImage cannot be duplicated
        TextureImage(const TextureImage& OtherTexture) = delete;
        TextureImage& operator=(TextureImage& OtherTexture) = delete;
        TextureImage(TextureImage&& OtherTexture) noexcept;
        TextureImage& operator=(TextureImage&& OtherTexture) noexcept;

        const unsigned char* GetTextureData();

        int GetTextureWidth();
        int GetTextureHeight();
        int GetNumberOfChannels();

        // @brief
        // Free texture resources
        ~TextureImage();

        unsigned int TextureID; // 0 means -1, so the low level pair is not already been created

    protected:

        unsigned char* TextureBytes;
        int Width, Height, NumberOfChannels;

    };
}
