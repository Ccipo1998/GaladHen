
#include "BVH.h"

#include "BVHNode.h"
#include <Core/Mesh.h>

#include <Math/Math.h>

namespace GaladHen
{
	BVH::BVH()
		: RootNode(0)
	{}

	void BVH::BuildBVH_InPlace(Mesh& mesh)
	{
		Nodes.clear();
		Nodes.reserve(mesh.Vertices.size() * 2 - 1); // the size of the BVH for N triangles has an upper limit: we can never have more than 2N-1 nodes, since N primitives in N leaves have no more than N/2 parents, N/4 grandparents and so on

		Nodes.emplace_back(BVHNode{});
		BVHNode& root = Nodes[RootNode];
		root.LeftOrFirst = 0;
		root.IndexCount = mesh.Indices.size();

		root.AABoundingBox.BuildAABB(mesh.Vertices, mesh.Indices, mesh.PrimitiveType, 0, mesh.Indices.size());

		// Recursive subdivision
		LongestAxisSubdivision(root, mesh);
	}

	void BVH::BuildBVH(const Scene& scene)
	{

	}

	BVHNode& BVH::GetRootNode()
	{
		return Nodes[RootNode];
	}

	BVHNode& BVH::GetNode(unsigned int index)
	{
		return Nodes[index];
	}

	unsigned int BVH::GetNodeNumber() const
	{
		return Nodes.size();
	}

	void BVH::LongestAxisSubdivision(BVHNode& node, Mesh& mesh)
	{
		// Check if we reached a leaf // TODO: controllare se è corretto
		int indexStep = (int)mesh.PrimitiveType + 1;
		if (node.IndexCount <= indexStep - 1)
		{
			return;
		}

		// Split plane and position
		unsigned int splitAxis = node.AABoundingBox.LongestAxis();
		float splitCoord = node.AABoundingBox.SplitCoordinateAlongAxis(splitAxis);

		// Divide the aabb in two halves
		unsigned int i = node.LeftOrFirst;
		unsigned int j = i + node.IndexCount - 1;
		while (i <= j)
		{
			glm::vec3 centroid = Math::TriangleCentroidPosition(
				mesh.Vertices[mesh.Indices[i]].Position,
				mesh.Vertices[mesh.Indices[i + 1]].Position,
				mesh.Vertices[mesh.Indices[i + 2]].Position);

			if (centroid[splitAxis] < splitCoord)
			{
				i += indexStep;
			}
			else
			{
				std::swap(mesh.Indices[i], mesh.Indices[j--]);
				std::swap(mesh.Indices[i + 1], mesh.Indices[j--]);
				std::swap(mesh.Indices[i + 2], mesh.Indices[j--]);
			}
		}

		// Stop split if one of the sides is empty
		int leftCount = i - node.LeftOrFirst;
		if (leftCount == 0 || leftCount == node.IndexCount) return;

		// Create child nodes
		Nodes.emplace_back(BVHNode{});
		Nodes.emplace_back(BVHNode{});
		unsigned int leftChildIndex = Nodes.size() - 2;
		unsigned int rightChildIndex = leftChildIndex + 1;
		BVHNode& leftNode = Nodes[leftChildIndex];
		BVHNode& rightNode = Nodes[rightChildIndex];
		leftNode.LeftOrFirst = node.LeftOrFirst;
		leftNode.IndexCount = leftCount;
		rightNode.LeftOrFirst = i;
		rightNode.IndexCount = node.IndexCount - leftCount;
		node.LeftOrFirst = leftChildIndex;
		node.IndexCount = 0; // it means that this node is not a leaf

		// Create child AABBs
		leftNode.AABoundingBox.BuildAABB(mesh.Vertices, mesh.Indices, mesh.PrimitiveType, leftNode.LeftOrFirst, leftNode.IndexCount);
		rightNode.AABoundingBox.BuildAABB(mesh.Vertices, mesh.Indices, mesh.PrimitiveType, rightNode.LeftOrFirst, rightNode.IndexCount);

		// Recursion call
		LongestAxisSubdivision(leftNode, mesh);
		LongestAxisSubdivision(rightNode, mesh);
	}
}
