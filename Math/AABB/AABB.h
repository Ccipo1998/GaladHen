
// Class of a Axis Aligned Bounding Box data structure

#pragma once

#include <vector>

#include <glm/glm.hpp>

namespace GaladHen
{
	struct MeshVertexData;
	class Mesh;
	class TransformQuat;
	enum class MeshPrimitive;

	enum class AABBSplitMethod
	{
		Midpoint = 0,
		PlaneCandidates = 1,
		SurfaceAreaHeuristic = 2
	};

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
		void BuildAABB(const std::vector<MeshVertexData>& vertices, const std::vector<unsigned int>& indices, MeshPrimitive primitiveType, unsigned int fromIndex, unsigned int countIndex);

		// @brief
		// Build the AABB for a set (or a subset) of meshes
		// @param meshes: the array of meshes
		// @param fromIndex: starting index
		// @param countIndex: number of indices to consider
		void BuildAABB(const std::vector<Mesh>& meshes, unsigned int fromIndex, unsigned int countIndex);

		// @brief
		// Update the AABB such that it includes a new point
		void BoundPoint(const glm::vec3& pointToBound);

		// @brief
		// Update the AABB such that it includes another axis aligned bounding box
		void BoundAABB(const AABB& aabb);

		// @brief
		// Return the longest axis of the AABB
		// @returns 0 (x axis), 1 (y axis) or 2 (z axis)
		unsigned int LongestAxis();

		// @brief
		// Calculate midpoint split coordinate along a specific axis
		float MidpointSplitAlongAxis(unsigned int axis);

		// @brief
		// Calculate the center of the aabb
		glm::vec3 Center();
		
		// @brief
		// Calculate the half of the total area of the aabb
		float Area();

		// @brief
		// Create the corresponding mesh (primitive type = line)
		Mesh ToMesh() const;

		// @brief
		// Update the corresponding mesh
		void ToMesh(Mesh& outMesh);

		glm::vec3 MinBound;
		glm::vec3 MaxBound;
	};
}
