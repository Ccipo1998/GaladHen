
// Struct and functionalities for texture data

#pragma once

#include <string>

namespace GaladHen
{
    class Texture;

    enum class TextureWrapping
    {
        Repeat = 0,
        ClampToBorder = 1,
        ClampToEdge = 2,
        MirroredRepeat = 3
    };

    enum class TextureFiltering
    {
        Linear = 0,
        Nearest = 1
    };

    enum class TextureMipMap
    {
        LinearLinear = 0,
        NearestNearest = 1,
        LinearNearest = 2,
        NearestLinear = 3
    };

    // texture rendering parameters
    struct TextureParameters
    {
        TextureParameters()
            : Texture(nullptr)
            , HorizontalWrapping(TextureWrapping::Repeat)
            , VerticalWrapping(TextureWrapping::Repeat)
            , Filtering(TextureFiltering::Linear)
            , MipMapMode(TextureMipMap::LinearLinear) // defaults 
            {}

        Texture* Texture;

        TextureWrapping HorizontalWrapping;
        TextureWrapping VerticalWrapping;
        TextureFiltering Filtering;
        TextureMipMap MipMapMode;
        std::string SamplerName;
    };
}
