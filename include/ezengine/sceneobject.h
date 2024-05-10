
#pragma once

#include <vector>
#include <ezengine/transform.h>

class Model;
class Material;

class SceneObject
{

public:

    // @brief
    // Default ctor for a model-less scene object
    SceneObject();

    // @brief
    // Ctor for a scene object with a model
    SceneObject(const Model* model);

    // @brief
    // Set a material for a specific mesh inside the model
    // @param meshIndex: the index of the material's target mesh inside the model
    // @param material: the material to associate to the mesh
    void SetMaterial(unsigned int meshIndex, Material* material);

    // @brief
    // Get the material associated with the mesh at a specific index inside the model
    // @param meshIndex: the index of the material's target mesh inside the model
    Material* GetMaterial(unsigned int meshIndex);

    // @brief
    // Drawcall on the scene object model using the materials of its meshes
    void Draw();

    TransformQuat Transform;

protected:

    const Model* SceneObjectModel;
    std::vector<Material*> SceneObjectMaterials; // the number of materials and the number of meshes inside the model are always the same: mesh <-> material

};
