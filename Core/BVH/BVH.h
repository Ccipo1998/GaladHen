
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

	class BVH
	{
	public:

		BVH();

		// @brief
		// Build the BVH for a mesh, changing order of indices inside it (in-place)
		void BuildBVH_InPlace(Mesh& mesh);

		// @brief
		// Build the BVH from a scene
		void BuildBVH(const Scene& scene);

		BVHNode& GetRootNode();
		
		BVHNode& GetNode(unsigned int index);

		unsigned int GetNodeNumber() const;

	protected:

		void LongestAxisSubdivision(BVHNode& node, Mesh& mesh);

		std::vector<BVHNode> Nodes;
		unsigned int RootNode;

	};
}
