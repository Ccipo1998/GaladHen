
#include "MeshGL.h"

#include <Renderer/LayerAPI/IShaderProgramAPI.h>

namespace GaladHen
{
    MeshGL::MeshGL()
        : VAO(0)
        , VBO(0)
        , EBO(0)
        {}

    void MeshGL::Draw(MaterialData& data, ShadingMode mode, IShaderProgramAPI* shader)
    {
        // shader functionalities
        shader->Use();
        shader->LoadMaterialData(data, )
        shader->SetShadingMode(mode);

        // draw
        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, NumberOfFaces, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void MeshGL::LoadMemoryGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices)
    {
        gl3wInit();

        // unload previous loaded memory
        FreeMemoryGPU();
        
        // we create the buffers
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glGenBuffers(1, &EBO);

        // VAO is made "active"
        glBindVertexArray(VAO);
        // we copy data in the VBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);
        // we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

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

        NumberOfFaces = indices.size();
    }

    void MeshGL::FreeMemoryGPU()
    {
        if (VAO)
        {
            glDeleteVertexArrays(1, &VAO);
            glDeleteBuffers(1, &VBO);
            glDeleteBuffers(1, &EBO);
        }
    }
}
