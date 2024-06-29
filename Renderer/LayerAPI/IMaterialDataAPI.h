
// This is a pure virtual class (interface) to define generic material data

#pragma once

#include <vector>
#include "ITextureAPI.h"

namespace GaladHen
{
    // material data parameter types
    struct IMaterialScalar
    {
        float Scalar;
    };
    struct IMaterialVector2
    {
        float Vector[2];
    };
    struct IMaterialVector3
    {
        float Vector[3];
    };
    struct IMaterialVector4
    {
        float Vector[4];
    };

    // material data interface
    struct IMaterialDataAPI
    {
        // void to avoid vectors of pointers
    };
}
