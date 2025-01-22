
#pragma once

#include <vector>
#include <memory>

#include <Math/Transform.h>
#include "Material.h"

namespace GaladHen
{
    class Model;

    class SceneObject
    {

    public:

        SceneObject(std::shared_ptr<Model> model);

        // @brief
        // Set a material for a specific mesh inside the model
        // @param meshIndex: the index of the material's target mesh inside the model
        // @param material: the material to associate to the mesh
        void SetMeshMaterialLink(unsigned int meshIndex, std::shared_ptr<Material> material);

        // @brief
        // Get the material associated with the mesh at a specific index inside the model
        // @param meshIndex: the index of the material's target mesh inside the model
        std::shared_ptr<Material> GetMaterial(unsigned int meshIndex) const;

        // @brief
        // Get scene object materials
        std::vector<std::shared_ptr<Material>> GetSceneObjectMaterials() const;

        std::shared_ptr<Model> GetSceneObjectModel() const;

        // @brief
        // Delete the association between the scene object and its model
        void ClearSceneObjectModel();

        Transform Transform;

    protected:

        std::shared_ptr<Model> SceneObjectModel;
        std::vector<std::shared_ptr<Material>> SceneObjectMaterials; // the number of materials and the number of meshes inside the model are always the same: mesh <-> material

    };
}
