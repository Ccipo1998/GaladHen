
// Structs and functionalities for materials

#pragma once

#include <string>
#include <vector>

#include <glm.hpp>

#include "GaladHen/Texture.h"

namespace GaladHen
{
    enum ShadingMode
    {
        SmoothShading = 0,
        FlatShading = 1
    };

    // Material data parameter types
    struct MaterialDataScalar
    {
        std::string Name;
        float Scalar;
    };
    struct MaterialDataVector
    {
        std::string Name;
        glm::vec3 Vector;
    };
    struct MaterialDataTexture
    {
        std::string Name;
        Texture Tex;
    };

    // Material data interface
    struct MaterialData
    {
        virtual std::vector<MaterialDataScalar> GetScalarData() = 0;
        virtual std::vector<MaterialDataVector> GetVectorData() = 0;
        virtual std::vector<MaterialDataTexture> GetTextureData() = 0;
    };
}
