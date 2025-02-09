
#include "SceneObject.h"
#include "Model.h"
#include <utils/Log.h>
#include <utility>

namespace GaladHen
{
    SceneObject::SceneObject(std::weak_ptr<Model> model)
        : Transform(GaladHen::Transform{})
        , SceneObjectModel(model)
    {
        // Number of materials = number of meshes
        if (model.expired())
        {
            SceneObjectMaterials.resize(0);
        }
        else
        {
            Model* rawModel = model.lock().get();
            SceneObjectMaterials.resize(rawModel->Meshes.size());
        }
    }

    void SceneObject::SetMeshMaterialLink(unsigned int meshIndex, std::weak_ptr<Material> material)
    {
        Model* model = SceneObjectModel.lock().get();
        if (model && model->Meshes.size() <= meshIndex)
        {
            return;
        }

        // We are sure the size of SceneObjectMaterials is always equal to the number of SceneObjectModel's meshes
        SceneObjectMaterials[meshIndex] = material;
    }

    std::weak_ptr<Material> SceneObject::GetMaterial(unsigned int meshIndex) const
    {
        Model* model = SceneObjectModel.lock().get();
        if (model && model->Meshes.size() <= meshIndex)
        {
            return std::weak_ptr<Material>{};
        }

        return SceneObjectMaterials[meshIndex];
    }

    std::vector<std::weak_ptr<Material>> SceneObject::GetSceneObjectMaterials() const
    {
        return SceneObjectMaterials;
    }

    std::weak_ptr<Model> SceneObject::GetSceneObjectModel() const
    {
        return SceneObjectModel;
    }

    void SceneObject::ClearSceneObjectModel()
    {
        SceneObjectModel.reset();
    }
}
