
// This is the class holding texture actual data (pixel colors and alpha)

#pragma once

#include <GL/gl3w.h>
#include <string>

// TODO: refactoring using a friend class "TextureManager" to manage textureunits

class TextureImage
{

public:

    TextureImage();

    // @brief
    // Create a texture image from an image file
    TextureImage(const std::string& textureImagePath);

    // TextureImage has not copy  constructor and assignments -> a TextureImage cannot be duplicated
    TextureImage(TextureImage& OtherTexture) = delete;
    TextureImage& operator=(TextureImage& OtherTexture) = delete;
    TextureImage(TextureImage&& OtherTexture) = default;
    TextureImage& operator=(TextureImage&& OtherTexture) noexcept = default;

    const unsigned char* GetTextureData() const;

    // @brief
    // Send texture bytes to GPU memory
    // @param textureFormat: the format to interpret the texture (SRGB, RGBA, ...)
    // @param internalFormat: the format to interpret each texture byte (GL_RGB, GL_RED, ...)
    void SendTextureDataToGPU(GLenum textureFormat = GL_SRGB8, GLenum internalFormat = GL_RGB);

    int GetTextureWidth() const;
    int GetTextureHeight() const;
    int GetNumberOfChannels() const;
    bool IsLoadedInGPU() const;
    GLuint GetTextureID() const;

    // @brief
    // Free texture resources
    ~TextureImage();

protected:

    unsigned char* TextureBytes;
    int Width, Height, NumberOfChannels;

    GLuint TextureID;
    bool IsLoaded; // flag for texture data loading on gpu memory

    // @brief
    // Load an image file as a sequence of bytes that can be send to gpu memory
    void LoadTexture(const std::string& texturePath);

};
