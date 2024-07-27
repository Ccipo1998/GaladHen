
// OpenGL specific material data

#pragma once

#include "Renderer/LayerAPI/IMaterialDataAPI.h"

namespace GaladHen
{
    class TextureGL;

    struct TextureDataGL : public ITextureDataAPI
    {
        TextureGL* Texture;
    };
}
