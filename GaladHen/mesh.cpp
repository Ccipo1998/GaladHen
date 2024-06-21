
#include "Mesh.h"

namespace GaladHen
{
    Mesh::Mesh()
        : MeshID(0)
        {}

    Mesh::Mesh(const Mesh& other) noexcept
        : MeshID(0)
        , Vertices(other.Vertices)
        , Indices(other.Indices)
        {}

    Mesh& Mesh::operator=(const Mesh& other) noexcept
    {
        MeshID = 0;
        Vertices = other.Vertices;
        Indices = other.Indices;

        return *this;
    }

    Mesh::Mesh(Mesh&& other) noexcept
        : MeshID(other.MeshID)
        , Vertices(std::move(other.Vertices))
        , Indices(std::move(other.Indices))
    {
        other.MeshID = 0;
    }

    Mesh& Mesh::operator=(Mesh&& other) noexcept
    {
        MeshID = other.MeshID;
        other.MeshID = 0;
        Vertices = std::move(other.Vertices);
        Indices = std::move(other.Indices);

        return *this;
    }
}
