
// Structs and functionalities for meshes

#pragma once

#include <glm/glm.hpp>

namespace GaladHen
{
    struct VertexData
    {
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UV;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        glm::vec4 Color;
    };

    enum class Primitive
    {
        Point = 0,
        Line = 1,
        Triangle = 2
    };
}
