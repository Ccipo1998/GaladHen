
// This is a pure virtual class (interface) to define basic functionalities of a mesh at API level

#pragma once

#include <vector>

#include "Common/MeshData.h"

namespace GaladHen
{
    class MaterialData;
    enum ShadingMode;
    class IShaderProgramAPI;

    class IMeshAPI
    {

    public:

        // @brief
        // Drawcall
        virtual void Draw(MaterialData* data, ShadingMode mode, IShaderProgramAPI* shader) = 0;

        // @brief
        // Load mesh data in GPU memory
        virtual void LoadMemoryGPU(const std::vector<Vertex>& vertices, const std::vector<unsigned int> indices) = 0;

        // @brief
        // Free mesh data from GPU memory
        virtual void FreeMemoryGPU() = 0;

    };
}
