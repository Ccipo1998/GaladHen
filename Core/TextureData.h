
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
        Nearest = 1,
        LinearLinear = 2,
        NearestNearest = 3,
        LinearNearest = 4,
        NearestLinear = 5
    };

    enum class TextureFormat
    {
        RGB = 0,
        RGB8 = 1,
        SRGB = 2,
        SRGB8
    };

    // texture rendering parameters
    struct TextureParameters
    {
        TextureParameters()
            : TextureSource(nullptr)
            , HorizontalWrapping(TextureWrapping::Repeat)
            , VerticalWrapping(TextureWrapping::Repeat)
            , MinFiltering(TextureFiltering::LinearLinear) // default
            , MagFiltering(TextureFiltering::Linear) // default
            {}

        Texture* TextureSource;

        TextureWrapping HorizontalWrapping;
        TextureWrapping VerticalWrapping;
        TextureFiltering MinFiltering;
        TextureFiltering MagFiltering;
    };
}
