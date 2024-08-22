
// Class and functionalities for a Bounding Volume Hierarchy data structure
// Reference: https://jacco.ompf2.com/

#pragma once

#include <vector>

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
		// Build the BVH from a mesh
		void BuildBVH(const Mesh& mesh);

		// @brief
		// Build the BVH from a scene
		void BuildBVH(const Scene& scene);

		const BVHNode& GetRootNode();

	protected:

		std::vector<BVHNode> Nodes;
		unsigned int RootNode;
	};
}
