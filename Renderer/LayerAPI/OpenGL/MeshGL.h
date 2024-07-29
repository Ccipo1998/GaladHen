
// Mesh class for OpenGL API

#pragma once

#include <Renderer/LayerAPI/IMeshAPI.h>

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

namespace GaladHen
{
    class MeshGL : public IMeshAPI
    {

    public:

        MeshGL();

        virtual void Draw(IShaderProgramAPI* shader) override;

        virtual void LoadMemoryGPU(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices) override;

        virtual void FreeMemoryGPU() override;

    protected:

        GLuint VAO, VBO, EBO;
        
        unsigned int NumberOfIndices;
    };
}
