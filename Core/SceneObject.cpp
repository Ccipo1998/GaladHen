
#include "SceneObject.h"
#include "Model.h"
#include <utils/Log.h>
#include <utility>

namespace GaladHen
{
    SceneObject::SceneObject()
        : Transform(TransformQuat{})
        , SceneObjectModel(nullptr) // TODO: get default model in assets manager
        // , SceneObjectMaterials() // TODO: get default materials in assets manager
        {}

    SceneObject::SceneObject(Model* model, std::vector<Material>& materials)
        : Transform(TransformQuat{})
    {
        SceneObjectModel = model;
        
        // allocate correct number of materials
        SceneObjectMaterials.resize(SceneObjectModel->Meshes.size(), Material{});

        // fill vectors with materials
        for (unsigned int i = 0; i < materials.size(); ++i)
        {
            SceneObjectMaterials[i] = materials[i];
        }

        if (model->Meshes.size() != materials.size())
            Log::Error("SceneObject", "Unmatch between materials and meshes number"); // TODO: warning
    }

    void SceneObject::SetMeshMaterialLink(unsigned int meshIndex, const Material& material)
    {
        if (SceneObjectMaterials.size() <= meshIndex)
        {
            Log::Error("SceneObject", "Tried to assign a material to a mesh index out of range"); // TODO: warning

            return;
        }

        SceneObjectMaterials[meshIndex] = material;
    }

    Material& SceneObject::GetMaterial(unsigned int meshIndex)
    {
        return SceneObjectMaterials[meshIndex];
    }

    std::vector<Material>& SceneObject::GetSceneObjectMaterials()
    {
        return SceneObjectMaterials;
    }

    Model* SceneObject::GetSceneObjectModel() const
    {
        return SceneObjectModel;
    }

    void SceneObject::ClearSceneObjectModel()
    {
        SceneObjectModel = nullptr;
    }
}
