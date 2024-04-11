
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
