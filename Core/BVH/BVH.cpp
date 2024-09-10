
#include "BVH.h"

#include "BVHNode.h"
#include <Core/Mesh.h>

#include <Math/Math.h>
#include <Math/Ray.h>

#include <stack>

namespace GaladHen
{
	BVH::BVH()
		: RootNode(0)
	{}

	void BVH::BuildBVH(Mesh& mesh, AABBSplitMethod splitMethod)
	{
		Nodes.clear();
		Nodes.reserve(mesh.Vertices.size() * 2 - 1); // the size of the BVH for N triangles has an upper limit: we can never have more than 2N-1 nodes, since N primitives in N leaves have no more than N/2 parents, N/4 grandparents and so on

		Nodes.emplace_back(BVHNode{});
		BVHNode& root = Nodes[RootNode];
		root.LeftOrFirst = 0;
		root.IndexCount = mesh.Indices.size();

		root.AABoundingBox.BuildAABB(mesh.Vertices, mesh.Indices, mesh.PrimitiveType, 0, mesh.Indices.size());

		// Recursive subdivision

		switch (splitMethod)
		{
		case GaladHen::AABBSplitMethod::Midpoint:

			LongestAxisMidpointSubdivision(root, mesh);

			break;
		case GaladHen::AABBSplitMethod::SurfaceAreaHeuristic:
		default:

			SAHSubdivision(root, mesh);

			break;
		}
	}

	void BVH::BuildBVH(const Scene& scene)
	{

	}

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, BVHTraversalMethod traversalMethod)
	{
		return CheckTriangleMeshIntersection(ray, mesh, GetRootNode(), traversalMethod);
	}

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const BVHNode& node, BVHTraversalMethod traversalMethod)
	{
		Ray internalUseRay = ray;

		switch (traversalMethod)
		{
		case GaladHen::BVHTraversalMethod::FrontToBack:

			return CheckTriangleMeshIntersection_FrontToBack(internalUseRay, mesh, node);

			break;
		case GaladHen::BVHTraversalMethod::OrientationInvariant:
		default:

			return CheckTriangleMeshIntersection_Recursive(internalUseRay, mesh, node);

			break;
		}
	}

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection(const Ray& ray, const Mesh& mesh, const unsigned int nodeIndex, BVHTraversalMethod traversalMethod)
	{
		Ray internalUseRay = ray;

		switch (traversalMethod)
		{
		case GaladHen::BVHTraversalMethod::FrontToBack:

			return CheckTriangleMeshIntersection_FrontToBack(internalUseRay, mesh, nodeIndex);

			break;
		case GaladHen::BVHTraversalMethod::OrientationInvariant:
		default:

			return CheckTriangleMeshIntersection_Recursive(internalUseRay, mesh, nodeIndex);

			break;
		}
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

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection_Recursive(Ray& ray, const Mesh& mesh, const BVHNode& node)
	{
		RayTriangleMeshHitInfo intersection{};

		if (!Math::RayAABBIntersection(ray, node.AABoundingBox).Hit())
			return intersection;

		if (node.IsLeaf())
		{
			// check intersection on geometry
			for (unsigned int i = node.LeftOrFirst; i < node.LeftOrFirst + node.IndexCount; i += 3) // TODO: questi test di intersezione dovrebbero dipendere dal tipo di primitive del BVH
			{
				RayTriangleHitInfo hit = Math::RayTriangleIntersection(
					ray,
					mesh.Vertices[mesh.Indices[i]].Position,
					mesh.Vertices[mesh.Indices[i + 1]].Position,
					mesh.Vertices[mesh.Indices[i + 2]].Position
				);

				if (hit.HitDistance < intersection.HitDistance)
				{
					*(RayTriangleHitInfo*)&intersection = hit;
					intersection.Index0 = mesh.Indices[i];
					intersection.Index1 = mesh.Indices[i + 1];
					intersection.Index2 = mesh.Indices[i + 2];
				}
			}
		}
		else
		{
			RayTriangleMeshHitInfo interLeft = CheckTriangleMeshIntersection_Recursive(ray, mesh, Nodes[node.LeftOrFirst]);
			if (interLeft.HitDistance < intersection.HitDistance)
			{
				intersection = interLeft;
			}

			RayTriangleMeshHitInfo interRight = CheckTriangleMeshIntersection_Recursive(ray, mesh, Nodes[node.LeftOrFirst + 1]);
			if (interRight.HitDistance < intersection.HitDistance)
			{
				intersection = interRight;
			}
		}

		// traversal optimization: shortening the ray means avoiding intersection tests that are further than the current intersection point
		ray.Length = intersection.HitDistance;

		return intersection;
	}

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection_Recursive(Ray& ray, const Mesh& mesh, const unsigned int nodeIndex)
	{
		return CheckTriangleMeshIntersection_Recursive(ray, mesh, Nodes[nodeIndex]);
	}

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection_FrontToBack(Ray& ray, const Mesh& mesh, const BVHNode& node)
	{
		RayTriangleMeshHitInfo bestHit{};

		std::stack<const BVHNode*> stackOfNodes;

		const BVHNode* currentNode = &node;
		while (true)
		{
			if (currentNode->IsLeaf())
			{
				for (unsigned int i = currentNode->LeftOrFirst; i < currentNode->LeftOrFirst + currentNode->IndexCount; i += 3)
				{
					RayTriangleHitInfo hit = Math::RayTriangleIntersection(
						ray,
						mesh.Vertices[mesh.Indices[i]].Position,
						mesh.Vertices[mesh.Indices[i + 1]].Position,
						mesh.Vertices[mesh.Indices[i + 2]].Position
					);

					if (hit.HitDistance < bestHit.HitDistance)
					{
						*(RayTriangleHitInfo*)&bestHit = hit;
						bestHit.Index0 = mesh.Indices[i];
						bestHit.Index1 = mesh.Indices[i + 1];
						bestHit.Index2 = mesh.Indices[i + 2];
					}
				}

				if (stackOfNodes.empty())
				{
					break;
				}
				else
				{
					currentNode = stackOfNodes.top();
					stackOfNodes.pop();
				}

				continue;
			}

			BVHNode* child1 = &Nodes[currentNode->LeftOrFirst];
			BVHNode* child2 = &Nodes[currentNode->LeftOrFirst + 1];

			RayHitInfo info1 = Math::RayAABBIntersection(ray, child1->AABoundingBox);
			RayHitInfo info2 = Math::RayAABBIntersection(ray, child2->AABoundingBox);

			if (info1.HitDistance > info2.HitDistance)
			{
				std::swap(info1, info2);
				std::swap(child1, child2);
			}

			if (!info1.Hit())
			{
				if (stackOfNodes.empty())
					break;

				currentNode = stackOfNodes.top();
				stackOfNodes.pop();
			}
			else
			{
				currentNode = child1;

				if (info2.Hit())
					stackOfNodes.push(child2);
			}
		}

		return bestHit;
	}

	RayTriangleMeshHitInfo BVH::CheckTriangleMeshIntersection_FrontToBack(Ray& ray, const Mesh& mesh, const unsigned int nodeIndex)
	{
		return RayTriangleMeshHitInfo{};
	}

	void BVH::LongestAxisMidpointSubdivision(BVHNode& node, Mesh& mesh)
	{
		// Check if we reached a leaf
		int primitive = (int)mesh.PrimitiveType + 1;
		if (node.IndexCount <= primitive - 1)
			return;

		// Split plane and position
		unsigned int splitAxis = node.AABoundingBox.LongestAxis();
		float splitCoord = node.AABoundingBox.MidpointSplitAlongAxis(splitAxis);

		// Divide the aabb in two halves
		int i = node.LeftOrFirst;
		int j = i + node.IndexCount - 1;
		while (i <= j)
		{
			glm::vec3 centroid = Math::TriangleCentroidPosition(
				mesh.Vertices[mesh.Indices[i]].Position,
				mesh.Vertices[mesh.Indices[i + 1]].Position,
				mesh.Vertices[mesh.Indices[i + 2]].Position);

			if (centroid[splitAxis] < splitCoord)
			{
				i += primitive;
			}
			else
			{
				std::swap(mesh.Indices[i], mesh.Indices[j]);
				std::swap(mesh.Indices[i + 1], mesh.Indices[j - 1]);
				std::swap(mesh.Indices[i + 2], mesh.Indices[j - 2]);

				j -= primitive;
			}
		}

		// Stop split if one of the sides is empty
		int leftCount = i - node.LeftOrFirst;
		if (leftCount == 0 || leftCount == node.IndexCount)
			return;

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
		LongestAxisMidpointSubdivision(leftNode, mesh);
		LongestAxisMidpointSubdivision(rightNode, mesh);
	}

	void BVH::SAHSubdivision(BVHNode& node, Mesh& mesh)
	{
		// Data for later check of recursion ending -> new method to detect when splitting is no longer convenient
		int primitive = (int)mesh.PrimitiveType + 1;
		float parentArea = node.AABoundingBox.Area(); // area of the parent's aabb
		float parentCost = node.IndexCount * parentArea;

		// Split plane and position
		unsigned int splitAxis;
		float splitCoord;
		float bestCost = LowestCostSplit_SAH(mesh, node, splitAxis, splitCoord);

		// Check if we reached a leaf
		if (parentCost <= bestCost)
			return;

		// Divide the aabb in two halves
		int i = node.LeftOrFirst;
		int j = i + node.IndexCount - 1;
		while (i <= j)
		{
			glm::vec3 centroid = Math::TriangleCentroidPosition(
				mesh.Vertices[mesh.Indices[i]].Position,
				mesh.Vertices[mesh.Indices[i + 1]].Position,
				mesh.Vertices[mesh.Indices[i + 2]].Position);

			if (centroid[splitAxis] < splitCoord)
			{
				i += primitive;
			}
			else
			{
				std::swap(mesh.Indices[i], mesh.Indices[j]);
				std::swap(mesh.Indices[i + 1], mesh.Indices[j - 1]);
				std::swap(mesh.Indices[i + 2], mesh.Indices[j - 2]);

				j -= primitive;
			}
		}

		// Stop split if one of the sides is empty
		int leftCount = i - node.LeftOrFirst;
		if (leftCount == 0 || leftCount == node.IndexCount)
			return;

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
		SAHSubdivision(leftNode, mesh);
		SAHSubdivision(rightNode, mesh);
	}

	float BVH::LowestCostSplit_SAH(const Mesh& mesh, const BVHNode& node, unsigned int& outAxis, float& outSplitCoordinate)
	{
		unsigned int bestAxis = 0;
		float bestCoord = 0;
		float bestCost = std::numeric_limits<float>::max();

		// testing all midpoints on all three axis
		int primitive = (int)mesh.PrimitiveType + 1;
		for (unsigned int axis = 0; axis < 3; axis++)
		{
			for (unsigned int i = node.LeftOrFirst; i < node.LeftOrFirst + node.IndexCount; i += primitive)
			{
				glm::vec3 candidatePos = Math::TriangleCentroidPosition(
					mesh.Vertices[mesh.Indices[i]].Position,
					mesh.Vertices[mesh.Indices[i + 1]].Position,
					mesh.Vertices[mesh.Indices[i + 2]].Position
				);

				float cost = EvaluateCostSAH(mesh, node, axis, candidatePos[axis]);

				if (cost < bestCost)
				{
					bestCoord = candidatePos[axis];
					bestAxis = axis;
					bestCost = cost;
				}
			}
		}

		// write outputs
		outAxis = bestAxis;
		outSplitCoordinate = bestCoord;

		return bestCost;
	}

	float BVH::EvaluateCostSAH(const Mesh& mesh, const BVHNode& node, unsigned int splitAxis, float splitCoordinate)
	{
		// build temp aabbs on which evaluate the SAH

		unsigned int leftCount = 0, rightCount = 0;
		int primitive = (int)mesh.PrimitiveType + 1;
		AABB leftAABB, rightAABB;

		for (unsigned int i = node.LeftOrFirst; i < node.LeftOrFirst + node.IndexCount; i += primitive)
		{
			const glm::vec3& v0 = mesh.Vertices[mesh.Indices[i]].Position;
			const glm::vec3& v1 = mesh.Vertices[mesh.Indices[i + 1]].Position;
			const glm::vec3& v2 = mesh.Vertices[mesh.Indices[i + 2]].Position;
			glm::vec3 centroid = Math::TriangleCentroidPosition(v0, v1, v2);

			if (centroid[splitAxis] < splitCoordinate)
			{
				++leftCount;
				leftAABB.BoundPoint(v0);
				leftAABB.BoundPoint(v1);
				leftAABB.BoundPoint(v2);
			}
			else
			{
				++rightCount;
				rightAABB.BoundPoint(v0);
				rightAABB.BoundPoint(v1);
				rightAABB.BoundPoint(v2);
			}
		}
		float cost = leftCount * leftAABB.Area() + rightCount * rightAABB.Area();
		return cost > 0.0f ? cost : std::numeric_limits<float>::max();
	}
}
