
#include "Mesh.h"

namespace GaladHen
{
	Mesh::Mesh(const std::vector<MeshVertexData>& vertices, const std::vector<unsigned int>& indices, MeshPrimitive primitive)
		: Vertices(vertices)
		, Indices(indices)
		, PrimitiveType(primitive)
	{}

	Mesh::Mesh(const Mesh& source)
	{
		Vertices = source.Vertices;
		Indices = source.Indices;
		PrimitiveType = source.PrimitiveType;

		InvalidateResource();
	}

	Mesh& Mesh::operator=(Mesh& source)
	{
		Vertices = source.Vertices;
		Indices = source.Indices;
		PrimitiveType = source.PrimitiveType;

		InvalidateResource();

		return *this;
	}

	Mesh::Mesh(Mesh&& source) noexcept
	{
		Vertices = std::move(source.Vertices);
		Indices = std::move(source.Indices);
		PrimitiveType = source.PrimitiveType;
	}

	Mesh& Mesh::operator=(Mesh&& source) noexcept
	{
		Vertices = std::move(source.Vertices);
		Indices = std::move(source.Indices);
		PrimitiveType = source.PrimitiveType;

		return *this;
	}

	const std::vector<MeshVertexData>& Mesh::GetVertices() const
	{
		return Vertices;
	}

	const std::vector<unsigned int>& Mesh::GetIndices() const
	{
		return Indices;
	}

	MeshPrimitive Mesh::GetPrimitive() const
	{
		return PrimitiveType;
	}
}
