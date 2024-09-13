
// Model class representing a collection of meshes

#pragma once

#include <string>
#include <vector>

#include "Mesh.h"

namespace GaladHen
{
    class Model
    {

    public:

        Model();

        // Copy constructor
        Model(const Model& model) noexcept;
        // Copy assignment
        Model& operator=(const Model& model) noexcept;
        // Move assignment
        Model& operator=(Model&& model) noexcept;
        // Move constructor
        Model(Model&& model) noexcept;

        void BuildModelBVH(AABBSplitMethod meshSplitMethod, AABBSplitMethod modelSplitMethod);

        std::vector<Mesh> Meshes;

        BVH ModelBVH;
    };
}
