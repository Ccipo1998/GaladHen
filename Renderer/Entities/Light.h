
#pragma once

#include <Math/Transform.h>

namespace GaladHen
{
    class Light
    {
    public:

        Light();

        Light(const glm::vec3& color, float intensity);

        Transform Transform;
        glm::vec3 Color;
        float Intensity;

    };
}
