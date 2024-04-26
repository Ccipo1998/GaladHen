
#include <ezengine/textureimage.h>


#include <ezengine/texture.h>
#include <ezengine/shader.h>
#include <utils/stb_image.h>
#include <utils/log.h>
#include <string>

TextureImage::TextureImage()
    : TextureBytes(nullptr)
    , Width(-1)
    , Height(-1)
    , NumberOfChannels(-1)
    , TextureUnit(-1)
    , IsLoaded(false)
    {}

void TextureImage::LoadTexture(const char* texturePath)
{
    this->TextureBytes = stbi_load(texturePath, &this->Width, &this->Height, &this->NumberOfChannels, 0);

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

void TextureImage::SendTextureDataToGPU(GLenum textureUnit)
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

    if (textureUnit > GL_MAX_COMBINED_TEXTURE_IMAGE_UNITS)
    {
        // texture unit out of limit -> error
        Log::Error("TextureImage", "Send texture data request for an illegal texture unit");

        return;
    }

    // assign texture unit
    this->TextureUnit = textureUnit;

    // create new texture object
    glGenTextures(1, &this->TextureID);
    // assign texture to a texture unit
    glActiveTexture(this->TextureUnit); // -> all next bind textures refers to this
    // // bind new texture object to texture target and current active texture unit
    glBindTexture(GL_TEXTURE_2D, this->TextureID);
    // allocate immutable storage
    glTexStorage2D(GL_TEXTURE_2D, 1, GL_SRGB8, this->Width, this->Height);
    // copy texture data to texture object
    glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, this->Width, this->Height, GL_RGB, GL_UNSIGNED_BYTE, this->TextureBytes);

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

int TextureImage::GetBindedTextureUnit() const
{
    return this->TextureUnit;
}

bool TextureImage::IsLoadedInGPU() const
{
    return this->IsLoaded;
}
