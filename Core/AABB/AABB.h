
// Class of a Axis Aligned Bounding Box data structure

#pragma once

#include <vector>

#include <glm/glm.hpp>

#include <Core/MeshData.h>

namespace GaladHen
{
	struct VertexData;
	class Mesh;
	class TransformQuat;

	struct AABB
	{
		AABB();

		// @brief
		// Build the AABB for a set (or a subset) of primitives
		// @param vertices: the array of vertices
		// @param indices: the array of indices representing the primitives (sets of 3 indices)
		// @param primitiveType: the primitive type represented by indices
		// @param fromIndex: starting index
		// @param countIndex: number of indices to consider
		void BuildAABB(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices, Primitive primitiveType, unsigned int fromIndex, unsigned int countIndex);

		// @brief
		// Update the AABB with a transform applied to min and max bounds
		void UpdateAABB(const TransformQuat& transform);

		// @brief
		// Return the longest axis of the AABB
		// @returns 0 (x axis), 1 (y axis) or 2 (z axis)
		unsigned int LongestAxis();

		// @brief
		// Calculate split position along a specific axis
		float SplitCoordinateAlongAxis(unsigned int axis);

		// @brief
		// Create the corresponding mesh (primitive type = line)
		Mesh ToMesh() const;

		// @brief
		// Update the corresponding mesh (assumption: mesh was created by ToMesh())
		void ToMeshUpdate(Mesh& outMesh);

		glm::vec3 MinBound;
		glm::vec3 MaxBound;
	};
}
