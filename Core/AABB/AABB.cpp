
#include "AABB.h"

#include <limits>

#include <Core/MeshData.h>
#include <Core/Mesh.h>

#include <Core/Transform.h>

namespace GaladHen
{
	AABB::AABB()
		: MinBound(0.0f)
		, MaxBound(0.0f)
	{}

	void AABB::BuildAABB(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices, Primitive primitiveType, unsigned int fromIndex, unsigned int countIndex)
	{
		int indicesStep = (int)primitiveType + 1;
		assert(indices.size() % indicesStep == 0); // correct number of indices for the primitive type

		MinBound = glm::vec3(std::numeric_limits<float>::max());
		MaxBound = glm::vec3(std::numeric_limits<float>::min());

		for (unsigned int i = fromIndex; i < fromIndex + countIndex; i += indicesStep)
		{
			BoundPoint(vertices[indices[i]].Position);
			BoundPoint(vertices[indices[i + 1]].Position);
			BoundPoint(vertices[indices[i + 2]].Position);
		}
	}

	void AABB::BuildAABB(const std::vector<Mesh>& meshes, unsigned int fromIndex, unsigned int countIndex)
	{

	}

	void AABB::BoundPoint(const glm::vec3& pointToBound)
	{
		MinBound = glm::min(MinBound, pointToBound);
		MaxBound = glm::max(MaxBound, pointToBound);
	}

	void AABB::BoundAABB(const AABB& aabb)
	{
		BoundPoint(aabb.MinBound);
		BoundPoint(aabb.MaxBound);
	}

	unsigned int AABB::LongestAxis()
	{
		glm::vec3 extent = MaxBound - MinBound;
		int axis = 0;
		if (extent.y > extent.x) axis = 1;
		if (extent.z > extent[axis]) axis = 2;

		return axis;
	}

	float AABB::MidpointSplitAlongAxis(unsigned int axis)
	{
		glm::vec3 extent = MaxBound - MinBound;
		return MinBound[axis] + extent[axis] * 0.5f;
	}

	glm::vec3 AABB::Center()
	{
		return (MaxBound + MinBound) * 0.5f;
	}

	float AABB::Area()
	{
		glm::vec3 extent = MaxBound - MinBound;
		return extent.x * extent.y + extent.y * extent.z + extent.z * extent.x;
	}

	Mesh AABB::ToMesh() const
	{
		Mesh mesh{};
		VertexData vertex;
		
		glm::vec3 extent = MaxBound - MinBound;

		// vertices

		vertex.Position = MinBound;
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(extent.x, 0.0f, 0.0f);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(0.0f, 0.0f, extent.z);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(0.0f, extent.y, 0.0f);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(extent.x, extent.y, 0.0f);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(0.0f, extent.y, extent.z);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(extent.x, 0.0f, extent.z);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MaxBound;
		mesh.Vertices.push_back(vertex);

		// indices

		mesh.Indices =
		{
			0, 1, 0, 2, 0, 3, 1, 4, 4, 3, 2, 5, 5, 3, 1, 6, 2, 6, 6, 7, 7, 5, 7, 4
		};

		mesh.PrimitiveType = Primitive::Line;

		return mesh;
	}

	void AABB::ToMeshUpdate(Mesh& outMesh)
	{
		VertexData vertex;

		glm::vec3 extent = MaxBound - MinBound;

		// vertices

		vertex.Position = MinBound;
		outMesh.Vertices[0] = vertex;

		vertex.Position = MinBound + glm::vec3(extent.x, 0.0f, 0.0f);
		outMesh.Vertices[1] = vertex;

		vertex.Position = MinBound + glm::vec3(0.0f, 0.0f, extent.z);
		outMesh.Vertices[2] = vertex;

		vertex.Position = MinBound + glm::vec3(0.0f, extent.y, 0.0f);
		outMesh.Vertices[3] = vertex;

		vertex.Position = MinBound + glm::vec3(extent.x, extent.y, 0.0f);
		outMesh.Vertices[4] = vertex;

		vertex.Position = MinBound + glm::vec3(0.0f, extent.y, extent.z);
		outMesh.Vertices[5] = vertex;

		vertex.Position = MinBound + glm::vec3(extent.x, 0.0f, extent.z);
		outMesh.Vertices[6] = vertex;

		vertex.Position = MaxBound;
		outMesh.Vertices[7] = vertex;

		// indices -> no change
	}
}
