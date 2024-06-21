
#include "Texture.h"

namespace GaladHen
{
    Texture::Texture()
        : TextureData(nullptr)
        , HorizontalWrapping(TextureWrapping::Repeat)
        , VerticalWrapping(TextureWrapping::Repeat)
        , Filtering(TextureFiltering::Linear)
        , MipMapMode(TextureMipMap::LinearLinear) // defaults
        {}
}
