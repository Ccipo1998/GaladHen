
#include "texture.h"
#include "shader.h"
#include "textureimage.h"
#include <utils/log.h>

// default values of texture parameters
#define DEFAULT_WRAPPING_MODE GL_REPEAT
#define DEFAULT_FILTERING_MODE GL_NEAREST
#define DEFAULT_MIPMAP_MODE GL_NEAREST_MIPMAP_NEAREST

Texture::Texture(const TextureImage* textureData)
    : TextureData(textureData)
    , WrappingModeX(DEFAULT_WRAPPING_MODE)
    , WrappingModeY(DEFAULT_WRAPPING_MODE)
    , FilteringMode(DEFAULT_FILTERING_MODE)
    , MipMapMode(DEFAULT_MIPMAP_MODE)
    {}

const TextureImage* Texture::GetTextureImage() const
{
    return this->TextureData;
}

void Texture::SetUniformSamplerForShader(const char* samplerName, const Shader* shader)
{
    if (this->TextureData == nullptr)
    {
        // no texture data -> error
        Log::Error("Texture", "Set uniform sampler request for an empty texture");

        return;
    }

    if (!this->TextureData->IsLoadedInGPU())
    {
        // texture image not loaded in memory -> error
        Log::Error("Texture", "Texture image must be loaded in GPU memory before creating samplers");

        return;
    }
    
    if (shader == nullptr)
    {
        // invalid shader -> error
        Log::Error("Texture", "Null shader");

        return;
    }

    // use shader
    shader->Use();

    // send texture parameters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->WrappingModeX);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->WrappingModeY);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->FilteringMode); // TODO: divide minifying and magnyfing filters
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->FilteringMode);
    glGenerateMipmap(GL_TEXTURE_2D);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->MipMapMode); // TODO: check if this is correct
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->MipMapMode); // TODO: divide minifying and magnyfing filters for mipmaps?

    // create uniform sampler
    int loc = glGetUniformLocation(shader->GetShaderProgram(), samplerName);
    glUniform1i(loc, 1);
}

void Texture::SetActiveTexture(GLenum textureUnit)
{
    if (this->TextureData == nullptr)
    {
        // void texture data
        Log::Error("Texture", "Set active texture request for a null texture image data");

        return;
    }

    if (!this->TextureData->IsLoadedInGPU())
    {
        // texture data not loaded in memory
        Log::Error("Texture", "Set active texture request for a texture image data not loaded in GPU memory yet");

        return;
    }

    if (textureUnit > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
    {
        // texture unit out of limit -> error
        Log::Error("Texture", "Send texture data request for an illegal texture unit");

        return;
    }

    this->TextureUnit = textureUnit;

    glActiveTexture(this->TextureUnit);
    glBindTexture(GL_TEXTURE_2D, this->TextureData->GetTextureID());
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
