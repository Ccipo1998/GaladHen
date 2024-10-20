
#include "Texture.h"
#include "TextureData.h"

#include <utility>

namespace GaladHen
{
    Texture::Texture()
        : TextureBytes(nullptr)
        , Width(-1)
        , Height(-1)
        , NumberOfChannels(-1)
        , TexFormat(TextureFormat::RGB)
        , NumberOfMipMaps(0)
        , TextureID(0)
        {}

    Texture::Texture(unsigned char* textureBytes, int width, int height, int numberOfChannels, TextureFormat textureFormat, unsigned int numberOfMipMaps)
        : TextureBytes(textureBytes)
        , Width(width)
        , Height(height)
        , NumberOfChannels(numberOfChannels)
        , TexFormat(textureFormat)
        , NumberOfMipMaps(numberOfMipMaps)
        , TextureID(0)
    {}

    Texture::Texture(Texture&& sourceTexture) noexcept
    {
        TextureBytes = sourceTexture.TextureBytes;
        Width = sourceTexture.Width;
        Height = sourceTexture.Height;
        NumberOfChannels = sourceTexture.NumberOfChannels;
        TexFormat = sourceTexture.TexFormat;
        NumberOfMipMaps = sourceTexture.NumberOfMipMaps;
        TextureID = sourceTexture.TextureID;

        sourceTexture.TextureBytes = nullptr;
        sourceTexture.Width = 0;
        sourceTexture.Height = 0;
        sourceTexture.NumberOfChannels = 0;
        sourceTexture.NumberOfMipMaps = 0;
        sourceTexture.TextureID = 0;
    }

    Texture& Texture::operator=(Texture&& sourceTexture) noexcept
    {
        TextureBytes = sourceTexture.TextureBytes;
        Width = sourceTexture.Width;
        Height = sourceTexture.Height;
        NumberOfChannels = sourceTexture.NumberOfChannels;
        TexFormat = sourceTexture.TexFormat;
        NumberOfMipMaps = sourceTexture.NumberOfMipMaps;
        TextureID = sourceTexture.TextureID;

        sourceTexture.TextureBytes = nullptr;
        sourceTexture.Width = 0;
        sourceTexture.Height = 0;
        sourceTexture.NumberOfChannels = 0;
        sourceTexture.NumberOfMipMaps = 0;
        sourceTexture.TextureID = 0;

        return *this;
    }

    const unsigned char* Texture::GetTextureData()
    {
        return TextureBytes;
    }

    int Texture::GetTextureWidth()
    {
        return Width;
    }

    int Texture::GetTextureHeight()
    {
        return Height;
    }

    int Texture::GetNumberOfChannels()
    {
        return NumberOfChannels;
    }

    TextureFormat Texture::GetTextureFormat()
    {
        return TexFormat;
    }

    Texture Texture::CreateTextureCopy()
    {
        // allocation
        int size = Width * Height * NumberOfChannels;
        unsigned char* allocation = new unsigned char[size];

        // copy data
        for (unsigned int i = 0; i < size; ++i)
        {
            allocation[i] = TextureBytes[i];
        }

        return Texture(allocation, Width, Height, NumberOfChannels, TexFormat, NumberOfMipMaps);
    }

    void Texture::FreeResources()
    {
        if (TextureBytes)
        {
            delete[] TextureBytes;
            TextureBytes = nullptr;
        }
    }

    Texture::~Texture()
    {
        FreeResources();
    }
}
