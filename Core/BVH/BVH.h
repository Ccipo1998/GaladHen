
// Class and functionalities for a Bounding Volume Hierarchy data structure
// Reference: https://jacco.ompf2.com/

// This is a non-sparse (contiguous) BVH implementation, because is based on non-sparse BVH node data structure

#pragma once

#include <vector>

#include <Core/MeshData.h>

namespace GaladHen
{
	struct BVHNode;
	class Mesh;
	class Scene;
	struct Ray;
	struct RayTriangleMeshHitInfo;
	struct AABB;
	enum class AABBSplitMethod;

	enum class BVHTraversalMethod
	{
		OrientationInvariant = 0,
		FrontToBack = 1
	};

	class BVH
	{
	public:

		BVH();

		// @brief
		// Build the BVH for a mesh, changing order of indices inside it (in-place)
		// @param mesh: the mesh to bound -> in place sort of elements inside the indices array
		// @param splitMethod: the aabb split method to use
		void BuildBVH(Mesh& mesh, AABBSplitMethod splitMethod);

		// @brief
		// Build the BVH from a scene
		void BuildBVH(const Scene& scene);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry
		// @param ray: the ray casted
		// @param mesh: the mesh used to perform intersection tests on actual geometry -> this MUST be the same mesh used when the bvh was builded
		// @param traversalMethod: the method to use for the traversal algorithm
		// @returns infos about intersection
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, BVHTraversalMethod traversalMethod);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry, strarting from a specific node
		// @param ray: the ray casted
		// @param mesh: the mesh used to perform intersection tests on actual geometry -> this MUST be the same mesh used when the bvh was builded
		// @param node: starting node of the intersection tests
		// @param traversalMethod: the method to use for the traversal algorithm
		// @returns infos about intersection
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVHNode& node, BVHTraversalMethod traversalMethod);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry, strarting from a specific node index
		// @param ray: the ray casted
		// @param mesh: the mesh used to perform intersection tests on actual geometry -> this MUST be the same mesh used when the bvh was builded
		// @param nodeIndex: starting node index of the intersection tests
		// @param traversalMethod: the method to use for the traversal algorithm
		// @returns infos about intersection
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const unsigned int nodeIndex, BVHTraversalMethod traversalMethod);

		BVHNode& GetRootNode();
		
		BVHNode& GetNode(unsigned int index);

		unsigned int GetNodeNumber() const;

		static unsigned int NumberOfCandidatePlanes;

	protected:

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry, strarting from a specific node.
		// Internal version for in-place modification of the ray, as optimization of the traversal algorithm
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection_Recursive(Ray& ray, const Mesh& mesh, const BVHNode& node);

		// @brief
		// Check if a ray intersects the bvh hierarchy and the triangle mesh's geometry, strarting from a specific node index
		// Internal version for in-place modification of the ray, as optimization of the traversal algorithm
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection_Recursive(Ray& ray, const Mesh& mesh, const unsigned int nodeIndex);

		RayTriangleMeshHitInfo CheckTriangleMeshIntersection_FrontToBack(Ray& ray, const Mesh& mesh, const BVHNode& node);
		RayTriangleMeshHitInfo CheckTriangleMeshIntersection_FrontToBack(Ray& ray, const Mesh& mesh, const unsigned int nodeIndex);

		void LongestAxisMidpointSubdivision(BVHNode& node, Mesh& mesh);

		void SAHSubdivision(BVHNode& node, Mesh& mesh);

		void PlaneCandidatesSubdivision(BVHNode& node, Mesh& mesh);

		// @brief
		// Calculate axis and position with lowest cost, basing on Surface Area Heuristic
		// @param mesh: source mesh used inside the BVH
		// @param node: the BVH node on which calculate the split
		// @param[out] outAxis: the axis to use for the split
		// @param[out] outSplitCoordinate: the coordinate along the axis where splitting is convenient
		// @return lowest cost of the split
		float LowestCostSplit_SAH(const Mesh& mesh, const BVHNode& node, unsigned int& outAxis, float& outSplitCoordinate);

		float BestSplitPlane(const Mesh& mesh, const BVHNode& node, unsigned int& outAxis, float& outSplitCoordinate);

		// @brief
		// Evaluate the cost function of the Surface Area Heuristic on given position and with given geometry
		float EvaluateCostSAH(const Mesh& mesh, const BVHNode& node, unsigned int splitAxis, float splitCoordinate);

		std::vector<BVHNode> Nodes;
		unsigned int RootNode;

	};
}
