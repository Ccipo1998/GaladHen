
#include <ezengine/texture.h>
#include <ezengine/shader.h>
#include <ezengine/textureimage.h>
#include <utils/log.h>

// default values of texture parameters
#define DEFAULT_WRAPPING_MODE GL_REPEAT
#define DEFAULT_FILTERING_MODE GL_NEAREST
#define DEFAULT_MIPMAP_MODE GL_NEAREST_MIPMAP_NEAREST

Texture::Texture(const TextureImage* textureData)
    : TextureData(textureData)
    , IsBinded(false)
    , WrappingModeX(DEFAULT_WRAPPING_MODE)
    , WrappingModeY(DEFAULT_WRAPPING_MODE)
    , FilteringMode(DEFAULT_FILTERING_MODE)
    , MipMapMode(DEFAULT_MIPMAP_MODE)
    {}

const TextureImage* Texture::GetTextureImage() const
{
    return this->TextureData;
}

void Texture::SendDataToShader(const unsigned char* samplerName, const Shader* shader)
{
    if (this->IsBinded)
    {
        // already binded -> error
        Log::Error("Texture", "Send data request for an already binded texture");

        return;
    }

    if (this->TextureData == nullptr)
    {
        // no texture data -> error
        Log::Error("Texture", "Send data request for an empty texture");

        return;
    }

    // create new texture object
    glGenTextures(1, &this->TextureID);

    // bind new texture object to texture target
    glBindTexture(GL_TEXTURE_2D, this->TextureID);

    // allocate immutable storage
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_RGBA8, this->TextureData->GetTextureWidth(), this->TextureData->GetTextureHeight());
    // copy texture data to texture object
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->TextureData->GetTextureWidth(), this->TextureData->GetTextureHeight(), GL_RGBA8, GL_UNSIGNED_BYTE, this->TextureData->GetTextureData());

    // send texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrappingModeX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrappingModeY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilteringMode); // TODO: divide minifying and magnyfing filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilteringMode);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->MipMapMode); // TODO: check if this is correct
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->MipMapMode); // TODO: divide minifying and magnyfing filters for mipmaps?

    IsBinded = true;
}

GLuint Texture::GetTextureID()
{
    return this->TextureID;
}

void Texture::SetRepeatWrappingX()
{
    this->WrappingModeX = GL_REPEAT;
}

void Texture::SetRepeatWrappingY()
{
    this->WrappingModeY = GL_REPEAT;
}

void Texture::SetMirrorRepeatWrappingX()
{
    this->WrappingModeX = GL_MIRRORED_REPEAT;
}

void Texture::SetMirrorRepeatWrappingY()
{
    this->WrappingModeY = GL_MIRRORED_REPEAT;
}

void Texture::SetClampToBorderWrappingX()
{
    this->WrappingModeX = GL_CLAMP_TO_BORDER;
}

void Texture::SetClampToBorderWrappingY()
{
    this->WrappingModeY = GL_CLAMP_TO_BORDER;
}

void Texture::SetClampToEdgeWrappingX()
{
    this->WrappingModeX = GL_CLAMP_TO_EDGE;
}

void Texture::SetClampToEdgeWrappingY()
{
    this->WrappingModeY = GL_CLAMP_TO_EDGE;
}

void Texture::SetLinearFiltering()
{
    this->FilteringMode = GL_LINEAR;
}

void Texture::SetNearestFiltering()
{
    this->FilteringMode = GL_NEAREST;
}

void Texture::SetNearestMipMap()
{
    this->MipMapMode = GL_NEAREST_MIPMAP_NEAREST;
}

void Texture::SetLinearMipMap()
{
    this->MipMapMode = GL_LINEAR_MIPMAP_LINEAR;
}

void Texture::SetNearestLinearMipMap()
{
    this->MipMapMode = GL_NEAREST_MIPMAP_LINEAR;
}

void Texture::SetLinearNearestMipMap()
{
    this->MipMapMode = GL_LINEAR_MIPMAP_NEAREST;
}
