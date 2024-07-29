
// This is a pure virtual class (interface) to define basic functionalities of a mesh at API level

#pragma once

#include <vector>

#include <Core/MeshData.h>
#include <Core/MaterialData.h>

namespace GaladHen
{
    class IShaderProgramAPI;

    class IMeshAPI
    {

    public:

        // @brief
        // Drawcall
        virtual void Draw(IShaderProgramAPI* shader) = 0;

        // @brief
        // Load mesh data in GPU memory
        virtual void LoadMemoryGPU(const std::vector<VertexData>& vertices, const std::vector<unsigned int>& indices) = 0;

        // @brief
        // Free mesh data from GPU memory
        virtual void FreeMemoryGPU() = 0;

    };
}
