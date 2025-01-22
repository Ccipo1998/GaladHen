
#pragma once

#include <vector>

#include "IGPUResource.h"
#include <glm/glm.hpp>
#include <Math/BVH/BVH.h>

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

	class Mesh : public IGPUResource
	{
        friend class BVH;
        friend struct AABB;

	public:

        Mesh(const std::vector<MeshVertexData>& vertices, const std::vector<unsigned int>& indices, MeshPrimitive primitive);

        Mesh(const Mesh& source);
        Mesh& operator=(Mesh& source);
        Mesh(Mesh&& source) noexcept;
        Mesh& operator=(Mesh&& source) noexcept;

        const std::vector<MeshVertexData>& GetVertices() const;
        const std::vector<unsigned int>& GetIndices() const;
        MeshPrimitive GetPrimitive() const;

        BVH BVH;

    protected:

        std::vector<MeshVertexData> Vertices;
        std::vector<unsigned int> Indices;
        MeshPrimitive PrimitiveType;

	};
}
