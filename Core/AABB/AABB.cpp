
#include "AABB.h"

#include <limits>

#include <Core/MeshData.h>
#include <Core/Mesh.h>

namespace GaladHen
{
	AABB::AABB()
		: MinBound(0.0f)
		, MaxBound(0.0f)
	{}

	void AABB::BuildAABB(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices, Primitive primitiveType)
	{
		int indicesStep = (int)primitiveType + 1;
		assert(indices.size() % indicesStep == 0); // correct number of indices for the primitive type

		MinBound = glm::vec3(std::numeric_limits<float>::max());
		MaxBound = glm::vec3(std::numeric_limits<float>::min());

		for (unsigned int i = 0; i < indices.size(); i += indicesStep)
		{
			MinBound = glm::min(MinBound, vertices[indices[i]].Position);
			MinBound = glm::min(MinBound, vertices[indices[i + 1]].Position);
			MinBound = glm::min(MinBound, vertices[indices[i + 2]].Position);
			MaxBound = glm::max(MaxBound, vertices[indices[i]].Position);
			MaxBound = glm::max(MaxBound, vertices[indices[i + 1]].Position);
			MaxBound = glm::max(MaxBound, vertices[indices[i + 2]].Position);
		}
	}

	Mesh AABB::ToMesh() const
	{
		Mesh mesh{};
		VertexData vertex;
		
		glm::vec3 differences = MaxBound - MinBound;

		// vertices

		vertex.Position = MinBound;
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(differences.x, 0.0f, 0.0f);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(0.0f, 0.0f, differences.z);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(0.0f, differences.y, 0.0f);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(differences.x, differences.y, 0.0f);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(0.0f, differences.y, differences.z);
		mesh.Vertices.push_back(vertex);

		vertex.Position = MinBound + glm::vec3(differences.x, 0.0f, differences.z);
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
}
