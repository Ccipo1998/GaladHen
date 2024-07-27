
// This is a pure virtual class (interface) to define generic material data

#pragma once

namespace GaladHen
{
    class TextureParameters;

    // material data interface
    struct ITextureDataAPI
    {
        TextureParameters* Parameters;
    };
}
