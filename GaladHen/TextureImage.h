
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
        // Create a texture image from an image file
        TextureImage(const std::string& textureImagePath);

        // TextureImage has not copy constructor and assignments -> a TextureImage cannot be duplicated
        TextureImage(TextureImage& OtherTexture) = delete;
        TextureImage& operator=(TextureImage& OtherTexture) = delete;
        TextureImage(TextureImage&& OtherTexture) = default;
        TextureImage& operator=(TextureImage&& OtherTexture) noexcept = default;

        const unsigned char* GetTextureData();

        int GetTextureWidth();
        int GetTextureHeight();
        int GetNumberOfChannels();

        // @brief
        // Free texture resources
        ~TextureImage();

    protected:

        void LoadTexture(const char* imagePath);

        unsigned char* TextureBytes;
        int Width, Height, NumberOfChannels;

    };
}
