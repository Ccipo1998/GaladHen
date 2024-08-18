
// Wrappers for generic material data

#pragma once

namespace GaladHen
{
    class TextureParameters;

    // texture data interface
    struct ITextureDataAPI
    {
        const char* SamplerName;
        const TextureParameters* Parameters;
        unsigned int TextureUnit;
    };
}
