
#pragma once

#include <vector>
#include "Transform.h"
#include "Material.h"

namespace GaladHen
{
    class Model;

    class SceneObject
    {

    public:

        // @brief
        // Default ctor for a scene object with a default model and material
        SceneObject();

        // Copy ctor
        SceneObject(const SceneObject& sceneObject) = default;
        // Copy assignment
        SceneObject& operator=(SceneObject& sceneObject) = default;
        // Move ctor
        SceneObject(SceneObject&& sceneObject) = default;
        // Move assignment
        SceneObject& operator=(SceneObject&& sceneObject) = default;

        // @brief
        // Ctor for a scene object with a model and with materials
        SceneObject(Model* model, std::vector<Material>& materials);

        // @brief
        // Set a material for a specific mesh inside the model
        // @param meshIndex: the index of the material's target mesh inside the model
        // @param material: the material to associate to the mesh
        void SetMeshMaterialLink(unsigned int meshIndex, const Material& material);

        // @brief
        // Get the material associated with the mesh at a specific index inside the model
        // @param meshIndex: the index of the material's target mesh inside the model
        Material& GetMaterial(unsigned int meshIndex);

        // @brief
        // Get scene object materials
        std::vector<Material>& GetSceneObjectMaterials();

        Model* GetSceneObjectModel() const;

        // @brief
        // Delete the association between the scene object and its model
        void ClearSceneObjectModel();

        TransformQuat Transform;

    protected:

        Model* SceneObjectModel;
        std::vector<Material> SceneObjectMaterials; // the number of materials and the number of meshes inside the model are always the same: mesh <-> material

    };
}
