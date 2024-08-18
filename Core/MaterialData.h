
// Structs and functionalities for materials

#pragma once

#include <string>
#include <vector>

#include <glm/glm.hpp>

#include "TextureData.h"

namespace GaladHen
{
    enum class ShadingMode
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
        MaterialDataTexture(std::string& name, TextureParameters textureParams)
        {
            Name = name;
            TexParams = textureParams;
        }

        std::string Name;
        TextureParameters TexParams;
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
