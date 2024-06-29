
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

        virtual void Draw(IMaterialDataAPI* data, ShadingMode mode, IShaderProgramAPI* shader) override;

        virtual void LoadMemoryGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) override;

        virtual void FreeMemoryGPU() override;

    protected:

        GLuint VAO, VBO, EBO;
        
        unsigned int NumberOfFaces;
    };
}
