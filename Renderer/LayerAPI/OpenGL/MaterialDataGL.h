
// OpenGL specific material data

#pragma once

#include "Renderer/LayerAPI/IMaterialDataAPI.h"
#include <utility>

namespace GaladHen
{
    class TextureGL;

    // material data parameter types with opengl specific data
    struct MaterialScalarGL : public IMaterialScalar
    {
        char* Name;
    };
    struct MaterialVector2GL : public IMaterialVector2
    {
        char* Name;
    };
    struct MaterialVector3GL : public IMaterialVector3
    {
        char* Name;
    };
    struct MaterialVector4GL : public IMaterialVector4
    {
        char* Name;
    };

    // material data interface
    struct MaterialDataGL : public IMaterialDataAPI
    {
        std::vector<MaterialScalarGL> ScalarData;
        std::vector<MaterialVector2GL> Vector2Data;
        std::vector<MaterialVector3GL> Vector3Data;
        std::vector<MaterialVector4GL> Vector4Data;
        std::vector<std::pair<TextureGL*, TextureParameters>> TextureData;
    };
}
