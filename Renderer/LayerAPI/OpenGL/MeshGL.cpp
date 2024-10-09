
#include "MeshGL.h"

#include <Renderer/LayerAPI/IShaderProgramAPI.h>

namespace GaladHen
{
    GLenum MeshGL::PrimitiveTypes[3] =
    {
        GL_POINTS,
        GL_LINES,
        GL_TRIANGLES
    };

    MeshGL::MeshGL()
        : VAO(0)
        , VBO(0)
        , EBO(0)
        , NumberOfIndices(0)
        {}

    void MeshGL::Draw(IShaderProgramAPI* shader, Primitive primitiveType)
    {
        // shader functionalities
        shader->Use();

        // draw
        glBindVertexArray(VAO);
        glDrawElements(PrimitiveTypes[(int)primitiveType], NumberOfIndices, GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
    }

    void MeshGL::LoadMemoryGPU(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices)
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
        glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(VertexData), &vertices[0], GL_STATIC_DRAW);
        // we copy data in the EBO - we must set the data dimension, and the pointer to the structure cointaining the data
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), &indices[0], GL_STATIC_DRAW);

        // we set in the VAO the pointers to the different vertex attributes (with the relative offsets inside the data structure)
        // vertex positions
        // these will be the positions to use in the layout qualifiers in the shaders ("layout (location = ...)"")
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)0);
        // Normals
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Normal));
        // Texture Coordinates
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, UV));
        // Tangent
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Tangent));
        // Bitangent
        glEnableVertexAttribArray(4);
        glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Bitangent));
        // Vertex color
        glEnableVertexAttribArray(5);
        glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, sizeof(VertexData), (GLvoid*)offsetof(VertexData, Color));

        glBindVertexArray(0);

        NumberOfIndices = indices.size();
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
