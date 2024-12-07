
// Datas about meshes

#pragma once

#include <vector>
#include <glm/glm.hpp>

namespace GaladHen
{
	struct MeshVertexData
	{
        glm::vec3 Position;
        glm::vec3 Normal;
        glm::vec2 UV;
        glm::vec3 Tangent;
        glm::vec3 Bitangent;
        glm::vec4 Color;
	};

    enum class MeshPrimitive
    {
        Point = 0,
        Line = 1,
        Triangle = 2
    };

    struct MeshData
    {
        std::vector<MeshVertexData> Vertices;
        std::vector<unsigned int> Indices;
        MeshPrimitive PrimitiveType;
    };
}
