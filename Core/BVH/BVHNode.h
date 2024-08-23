
// Data structure for a non-sparse (contiguous) BVH node, where each single node points always to adjacent-only data

#pragma once

#include <Core/AABB/AABB.h>

namespace GaladHen
{
	struct BVHNode
	{
		bool IsLeaf() const
		{
			return IndexCount != 0;
		}

		AABB AABoundingBox;
		unsigned int LeftOrFirst; // LeftChildIndex when IndexCount = 0, FirstIndex otherwise
		unsigned int IndexCount; // assumption: FirstIndex ans IndexCount represents adjacent data
	};
}
