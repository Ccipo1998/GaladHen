
#include "mesh.h"
#include "material.h"
#include <utils/log.h>

Mesh::Mesh(std::vector<Vertex>& vertices, std::vector<GLuint>& indices) noexcept
    : Vertices(move(vertices))
    , Indices(move(indices))
{
    this->InitGPUmemory();
}

Mesh::Mesh(Mesh&& other) noexcept
    // Calls move for both vectors, which internally consists of a simple pointer swap between the new instance and the source one.
    : Vertices(move(other.Vertices))
    , Indices(move(other.Indices))
    , VAO(other.VAO)
    , VBO(other.VBO)
    , EBO(other.EBO)
{
    other.VAO = 0; // We *could* set VBO and EBO to 0 too,
    // but since we bring all the 3 values around we can use just one of them to check ownership of the 3 resources.
}

Mesh& Mesh::operator=(Mesh&& other) noexcept
{
    // skip if we are trying to assign an object to itself
    if (this == &other)
        return *this;

    // calls the function which will delete (if needed) the GPU resources for this instance
    this->FreeGPUresources();

    if (other.VAO) // source instance has GPU resources
    {
        Vertices = move(other.Vertices);
        Indices = move(other.Indices);
        VAO = other.VAO;
        VBO = other.VBO;
        EBO = other.EBO;

        other.VAO = 0;
    }
    else // source instance was already invalid
    {
        VAO = 0;
    }
    
    return *this;
}

Mesh::~Mesh() noexcept
{
    // calls the function which will delete (if needed) the GPU resources
    this->FreeGPUresources();
}

void Mesh::Draw() const
{
    glBindVertexArray(this->VAO);
    glDrawElements(GL_TRIANGLES, this->Indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Mesh::Draw(const Material* material) const
{
    if (material == nullptr)
    {
        Log::Error("Mesh", "Impossible to execute the drawcall on the mesh: null material");

        return;
    }

    // send material data to its shader
    material->SendDataToShader();
    // use shader
    material->UseShader();

    this->Draw();
}

void Mesh::InitGPUmemory()
{
    gl3wInit(); // -> only on windows, i dont know why (otherwise the program crashes)
    // we create the buffers
    glGenVertexArrays(1, &this->VAO);
    glGenBuffers(1, &this->VBO);
    glGenBuffers(1, &this->EBO);

    // VAO is made "active"
    glBindVertexArray(this->VAO);
    // we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
    glBindBuffer(GL_ARRAY_BUFFER, this->VBO);
    glBufferData(GL_ARRAY_BUFFER, this->Vertices.size() * sizeof(Vertex), &this->Vertices[0], GL_STATIC_DRAW);
    // we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, this->EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, this->Indices.size() * sizeof(GLuint), &this->Indices[0], GL_STATIC_DRAW);

    // we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
    // vertex positions
    // these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)0);
    // Normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Normal));
    // Texture Coordinates
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, UV));
    // Tangent
    glEnableVertexAttribArray(3);
    glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Tangent));
    // Bitangent
    glEnableVertexAttribArray(4);
    glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (GLvoid*)offsetof(Vertex, Bitangent));

    glBindVertexArray(0);
}

void Mesh::FreeGPUresources()
{
    // If VAO is 0, this instance of Mesh has been through a move, and no longer owns GPU resources,
    // so there's no need for deleting.
    if (VAO)
    {
        glDeleteVertexArrays(1, &this->VAO);
        glDeleteBuffers(1, &this->VBO);
        glDeleteBuffers(1, &this->EBO);
    }
}
