
#include "TextureImage.h"

#include <Utils/stb_image.h>
#include <Utils/Log.h>

namespace GaladHen
{
    TextureImage::TextureImage()
        : TextureBytes(nullptr)
        , Width(-1)
        , Height(-1)
        , NumberOfChannels(-1)
        {}

    TextureImage::TextureImage(const std::string& textureImagePath)
    {
        LoadTexture(textureImagePath.data());
    }

    void TextureImage::LoadTexture(const char* imagePath)
    {
        TextureBytes = stbi_load(imagePath, &Width, &Height, &NumberOfChannels, 0);

        if (TextureBytes == nullptr)
        {
            std::string error;
            error.append("Unable to open file with path: ");
            error.append(imagePath);
            Log::Error("TextureImage", error);
        }
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
