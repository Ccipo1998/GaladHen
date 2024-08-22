
// Node of the BVH data structure

#pragma once

#include <Core/AABB/AABB.h>

namespace GaladHen
{
	struct BVHNode
	{
		AABB AABoundingBox;
		unsigned int LeftChild, RightChild;
		unsigned int FirstIndex, IndexCount;
	};
}
