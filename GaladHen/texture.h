
// This is a class pointing to a texture, exposing texture parameters for rendering

#pragma once

#include <Common/TextureData.h>

namespace GaladHen
{
    class Shader;
    class TextureImage;

    class Texture
    {

    public:

        Texture();

        // @brief
        // Create a texture
        // @param textureData: pointer to the texture image to use inside the texture
        // @param hWrapping: horizontal wrapping mode
        // @param vWrapping: vertical wrapping mode
        // @param filtering: filtering mode
        // @param modeMipMap: mip map mode
        Texture(TextureImage* textureData, TextureWrapping hWrapping, TextureWrapping vWrapping, TextureFiltering filtering, TextureMipMap modeMipMap);

        Texture(Texture& other) = default;
        Texture& operator=(Texture& other) = default;
        Texture(Texture&& other) = default;
        Texture& operator=(Texture&& other) = default;

        TextureImage* TextureData;
        // texture rendering parameters
        TextureParameters Parameters;

    };
}
