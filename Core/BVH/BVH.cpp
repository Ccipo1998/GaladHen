
#include "BVH.h"

#include "BVHNode.h"
#include <Core/Mesh.h>

namespace GaladHen
{
	BVH::BVH()
		: RootNode(0)
	{}

	void BVH::BuildBVH(const Mesh& mesh)
	{
		Nodes.clear();
		Nodes.reserve(mesh.Vertices.size() * 2 - 1); // the size of the BVH for N triangles has an upper limit: we can never have more than 2N-1 nodes, since N primitives in N leaves have no more than N/2 parents, N/4 grandparents and so on

		Nodes.emplace_back(BVHNode{});
		BVHNode& root = Nodes[RootNode];
		root.FirstIndex = 0;
		root.IndexCount = mesh.Indices.size();

		root.AABoundingBox.BuildAABB(mesh.Vertices, mesh.Indices, mesh.PrimitiveType);
	}

	void BVH::BuildBVH(const Scene& scene)
	{

	}

	const BVHNode& BVH::GetRootNode()
	{
		return Nodes[RootNode];
	}
}
