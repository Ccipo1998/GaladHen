
// Class of a Axis Aligned Bounding Box data structure

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Core/MeshData.h>

namespace GaladHen
{
	struct VertexData;
	class Mesh;

	struct AABB
	{
		AABB();

		// @brief
		// Build the AABB for a set of primitives
		// @param vertices: the array of vertices
		// @param indices: the array of indices representing the primitives (sets of 3 indices)
		// @param primitiveType: the primitive type represented by indices
		void BuildAABB(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices, Primitive primitiveType);

		// @brief
		// Create the corresponding mesh (primitive type = line)
		Mesh ToMesh() const;

		glm::vec3 MinBound;
		glm::vec3 MaxBound;
	};
}
