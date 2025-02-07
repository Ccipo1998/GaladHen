
#pragma once

#include <Math/Transform.h>

namespace GaladHen
{
    class Light
    {
    public:

        Light();

        Light(const glm::vec4& color, float intensity);

        Transform Transform;
        glm::vec4 Color;
        float Intensity;

    };
}
