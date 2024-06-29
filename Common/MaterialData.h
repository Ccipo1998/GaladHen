
// Structs and functionalities for materials

#pragma once

#include <string>
#include <vector>

#include "Libs/glm/glm.hpp"

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
    struct MaterialDataVector2
    {
        std::string Name;
        glm::vec2 Vector;
    };
    struct MaterialDataVector3
    {
        std::string Name;
        glm::vec3 Vector;
    };
    struct MaterialDataVector4
    {
        std::string Name;
        glm::vec4 Vector;
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
        virtual std::vector<MaterialDataVector2> GetVector2Data() = 0;
        virtual std::vector<MaterialDataVector3> GetVector3Data() = 0;
        virtual std::vector<MaterialDataVector4> GetVector4Data() = 0;
        virtual std::vector<MaterialDataTexture> GetTextureData() = 0;
    };
}
