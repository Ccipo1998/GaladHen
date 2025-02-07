
#include "SceneObject.h"
#include "Model.h"
#include <utils/Log.h>
#include <utility>

namespace GaladHen
{
    SceneObject::SceneObject(std::shared_ptr<Model> model)
        : Transform(GaladHen::Transform{})
        , SceneObjectModel(model)
    {
        // Number of materials = number of meshes
        if (model.get() == nullptr)
        {
            SceneObjectMaterials.resize(0);
        }
        else
        {
            SceneObjectMaterials.resize(model->GetMeshes().size());
        }
    }

    void SceneObject::SetMeshMaterialLink(unsigned int meshIndex, std::shared_ptr<Material> material)
    {
        if (SceneObjectModel->GetMeshes().size() <= meshIndex)
        {
            return;
        }

        // We are sure the size of SceneObjectMaterials is always equal to the number of SceneObjectModel's meshes
        SceneObjectMaterials[meshIndex] = material;
    }

    std::shared_ptr<Material> SceneObject::GetMaterial(unsigned int meshIndex) const
    {
        if (SceneObjectModel->GetMeshes().size() <= meshIndex)
        {
            return nullptr;
        }

        return SceneObjectMaterials[meshIndex];
    }

    std::vector<std::shared_ptr<Material>> SceneObject::GetSceneObjectMaterials() const
    {
        return SceneObjectMaterials;
    }

    std::shared_ptr<Model> SceneObject::GetSceneObjectModel() const
    {
        return SceneObjectModel;
    }

    void SceneObject::ClearSceneObjectModel()
    {
        SceneObjectModel = nullptr;
    }
}
