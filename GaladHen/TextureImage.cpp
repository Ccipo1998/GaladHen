
#include "TextureImage.h"

#include <Utils/stb_image.h>

namespace GaladHen
{
    TextureImage::TextureImage()
        : TextureBytes(nullptr)
        , Width(-1)
        , Height(-1)
        , NumberOfChannels(-1)
        , TextureID(0)
        {}

    TextureImage::TextureImage(unsigned char* textureBytes, int widht, int height, int channels)
        : TextureBytes(textureBytes)
        , Width(widht)
        , Height(height)
        , NumberOfChannels(channels)
        , TextureID(0)
        {}

    TextureImage::TextureImage(TextureImage&& OtherTexture) noexcept
    {
        TextureBytes = OtherTexture.TextureBytes;
        Width = OtherTexture.Width;
        Height = OtherTexture.Height;
        NumberOfChannels = OtherTexture.NumberOfChannels;
        TextureID = OtherTexture.TextureID;

        OtherTexture.TextureBytes = nullptr;
        OtherTexture.Width = 0;
        OtherTexture.Height = 0;
        OtherTexture.NumberOfChannels = 0;
        OtherTexture.TextureID = 0;
    }

    TextureImage& TextureImage::operator=(TextureImage&& OtherTexture) noexcept
    {
        TextureBytes = OtherTexture.TextureBytes;
        Width = OtherTexture.Width;
        Height = OtherTexture.Height;
        NumberOfChannels = OtherTexture.NumberOfChannels;
        TextureID = OtherTexture.TextureID;

        OtherTexture.TextureBytes = nullptr;
        OtherTexture.Width = 0;
        OtherTexture.Height = 0;
        OtherTexture.NumberOfChannels = 0;
        OtherTexture.TextureID = 0;

        return *this;
    }

    const unsigned char* TextureImage::GetTextureData()
    {
        return TextureBytes;
    }

    int TextureImage::GetTextureWidth()
    {
        return Width;
    }

    int TextureImage::GetTextureHeight()
    {
        return Height;
    }

    int TextureImage::GetNumberOfChannels()
    {
        return NumberOfChannels;
    }

    TextureImage::~TextureImage()
    {
        stbi_image_free(TextureBytes);
    }
}
