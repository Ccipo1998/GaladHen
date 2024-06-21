
// This is a class pointing to a texture, exposing texture parameters for rendering

#pragma once

namespace GaladHen
{
    class Shader;
    class TextureImage;

    enum TextureWrapping
    {
        Repeat = 0,
        ClampToBorder = 1,
        ClampToEdge = 2,
        MirroredRepeat = 3
    };

    enum TextureFiltering
    {
        Linear = 0,
        Nearest = 1
    };

    enum TextureMipMap
    {
        LinearLinear = 0,
        NearestNearest = 1,
        LinearNearest = 2,
        NearestLinear = 3
    };

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
        TextureWrapping HorizontalWrapping;
        TextureWrapping VerticalWrapping;
        TextureFiltering Filtering;
        TextureMipMap MipMapMode;

    };
}
