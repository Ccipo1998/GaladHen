
// Data structure for a non-sparse (contiguous) BVH node, where each single node points always to adjacent-only data

#pragma once

#include <Core/AABB/AABB.h>

namespace GaladHen
{
	struct BVHNode
	{
		AABB AABoundingBox;
		unsigned int LeftChild, RightChild;
		unsigned int FirstIndex, IndexCount; // assumption: adjacent data
	};
}
