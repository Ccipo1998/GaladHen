
// This is the class holding texture actual data (pixel colors and alpha)

#include <GL/gl3w.h>

class TextureImage
{

public:

    // @brief
    // Default constructor for a void TextureImage
    TextureImage();

    // TextureImage has not copy and move constructors and assignments -> a TextureImage can be created only calling explicitly LoadTexture
    TextureImage(TextureImage& OtherTexture) = delete;
    TextureImage& operator=(TextureImage& OtherTexture) = delete;
    TextureImage(TextureImage&& OtherTexture) = delete;
    TextureImage&& operator=(TextureImage&& OtherTexture) = delete;

    // @brief
    // Load an image file as a sequence of bytes that can be send to gpu memory
    void LoadTexture(const char* texturePath);

    const unsigned char* GetTextureData() const;

    int GetTextureWidth() const;
    int GetTextureHeight() const;
    int GetNumberOfChannels() const;

    // @brief
    // Free texture resources
    ~TextureImage();

protected:

    unsigned char* TextureBytes;
    int Width, Height, NumberOfChannels;

};
