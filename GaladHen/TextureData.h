
// Struct and functionalities for texture data

#pragma once

#include <string>

namespace GaladHen
{
    enum class TextureFormat
    {
        RGB = 0,
        SRGB = 1
    };

    enum class PixelDataFormat
    {
        R = 0,
        RG = 1,
        RGB = 2,
        RGBA = 3
    };

    enum class PixelDataType
    {
        UnsignedByte = 0
    };

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
            : HorizontalWrapping(TextureWrapping::Repeat)
            , VerticalWrapping(TextureWrapping::Repeat)
            , Filtering(TextureFiltering::Linear)
            , MipMapMode(TextureMipMap::LinearLinear) // defaults 
            {}

        TextureWrapping HorizontalWrapping;
        TextureWrapping VerticalWrapping;
        TextureFiltering Filtering;
        TextureMipMap MipMapMode;
        std::string SamplerName;
    };
}
