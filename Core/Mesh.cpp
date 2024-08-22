
#include "Mesh.h"
#include "MeshData.h"

namespace GaladHen
{
    Mesh::Mesh()
        : MeshID(0)
        , PrimitiveType(Primitive::Triangle)
        {}

    Mesh::Mesh(const Mesh& other) noexcept
        : MeshID(0)
        , Vertices(other.Vertices)
        , Indices(other.Indices)
        , PrimitiveType(other.PrimitiveType)
        {}

    Mesh& Mesh::operator=(const Mesh& other) noexcept
    {
        MeshID = 0;
        Vertices = other.Vertices;
        Indices = other.Indices;
        PrimitiveType = other.PrimitiveType;

        return *this;
    }

    Mesh::Mesh(Mesh&& other) noexcept
        : MeshID(other.MeshID)
        , Vertices(std::move(other.Vertices))
        , Indices(std::move(other.Indices))
        , PrimitiveType(other.PrimitiveType)
    {
        other.MeshID = 0;
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept
    {
        MeshID = other.MeshID;
        other.MeshID = 0;
        Vertices = std::move(other.Vertices);
        Indices = std::move(other.Indices);
        PrimitiveType = other.PrimitiveType;

        return *this;
    }
}
