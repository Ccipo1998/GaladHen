
#include "textureimage.h"

#include "texture.h"
#include "shader.h"
#include <utils/stb_image.h>
#include <utils/log.h>

TextureImage::TextureImage()
    : TextureBytes(nullptr)
    , Width(-1)
    , Height(-1)
    , NumberOfChannels(-1)
    , IsLoaded(false)
    {}

TextureImage::TextureImage(const std::string& textureImagePath)
{
    this->LoadTexture(textureImagePath);
}

void TextureImage::LoadTexture(const std::string& texturePath)
{
    this->TextureBytes = stbi_load(texturePath.data(), &this->Width, &this->Height, &this->NumberOfChannels, 0);

    if (this->TextureBytes == nullptr)
    {
        std::string error;
        error.append("Unable to open file with path: ");
        error.append(texturePath);
        Log::Error("TextureImage", error);
    }
}

const unsigned char* TextureImage::GetTextureData() const
{
    return this->TextureBytes;
}

GLuint TextureImage::GetTextureID() const
{
    return this->TextureID;
}

void TextureImage::SendTextureDataToGPU(GLenum textureFormat, GLenum internalFormat)
{
    if (this->IsLoaded)
    {
        // already loaded -> error
        Log::Error("TextureImage", "Send texture data request for an already loaded texture");

        return;
    }

    if (this->TextureBytes == nullptr)
    {
        // no texture data -> error
        Log::Error("TextureImage", "Send texture data request for an empty texture");

        return;
    }

    // create new texture object
    glGenTextures(1, &this->TextureID);
    // bind new texture object to texture target and current active texture unit
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    // allocate immutable storage basing on number of channels and on bit depth
    
    // IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
    glTexStorage2D(GL_TEXTURE_2D, 1, textureFormat, this->Width, this->Height);
    // copy texture data to texture object
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->Width, this->Height, internalFormat, GL_UNSIGNED_BYTE, this->TextureBytes);
    // if (this->NumberOfChannels == 3)
    // {
    //     // IMPORTANT: internal format is an external variable because not all the textures need to be interpreted as SRGB (example: normal maps are already stored in linear values)
    //     glTexStorage2D(GL_TEXTURE_2D, 1, internalFormat, this->Width, this->Height);
    //     // copy texture data to texture object
    //     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->Width, this->Height, GL_RGB, GL_UNSIGNED_BYTE, this->TextureBytes);
    // }
    // else if (this->NumberOfChannels == 1)
    // {
    //     glTexStorage2D(GL_TEXTURE_2D, 1, GL_R16, this->Width, this->Height);
    //     // copy texture data to texture object
    //     glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->Width, this->Height, GL_RED, GL_UNSIGNED_BYTE, this->TextureBytes);
    // }

    glGenerateTextureMipmap(this->TextureID);

    IsLoaded = true;
}

int TextureImage::GetTextureWidth() const
{
    return this->Width;
}

int TextureImage::GetTextureHeight() const
{
    return this->Height;
}

int TextureImage::GetNumberOfChannels() const
{
    return this->NumberOfChannels;
}

TextureImage::~TextureImage()
{
    stbi_image_free(this->TextureBytes);
}

bool TextureImage::IsLoadedInGPU() const
{
    return this->IsLoaded;
}
