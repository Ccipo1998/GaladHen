
#include "Model.h"
#include "Mesh.h"

namespace GaladHen
{
    Model::Model() {}

    Model::Model(const Model& model) noexcept
        : Meshes(model.Meshes)
        {}

    Model& Model::operator=(const Model& model) noexcept
    {
        Meshes = model.Meshes;

        return *this;
    }

    Model& Model::operator=(Model&& model) noexcept
    {
        Meshes = std::move(model.Meshes);

        return *this;
    }

    Model::Model(Model&& model) noexcept
        : Meshes(std::move(model.Meshes))
        {}
}

