
// This is a pure virtual class (interface) to define basic functionalities of a mesh at API level

#pragma once

#include <vector>

#include <GaladHen/MeshData.h>
#include <GaladHen/MaterialData.h>

namespace GaladHen
{
    class IMaterialDataAPI;
    class IShaderProgramAPI;

    class IMeshAPI
    {

    public:

        // @brief
        // Drawcall
        virtual void Draw(IMaterialDataAPI* data, ShadingMode mode, IShaderProgramAPI* shader) = 0;

        // @brief
        // Load mesh data in GPU memory
        virtual void LoadMemoryGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int>& indices) = 0;

        // @brief
        // Free mesh data from GPU memory
        virtual void FreeMemoryGPU() = 0;

    };
}
