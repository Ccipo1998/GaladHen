
#include "SceneObject.h"
#include "Model.h"
#include <utils/Log.h>
#include <utility>

namespace GaladHen
{
    SceneObject::SceneObject()
        : Transform(TransformQuat{})
        // , SceneObjectModel() // TODO: get default model in assets manager
        // , SceneObjectMaterials() // TODO: get default materials in assets manager
        {}

    SceneObject::SceneObject(SceneObject& sceneObject) noexcept
        : Transform(sceneObject.Transform)
        , SceneObjectModel(sceneObject.SceneObjectModel)
        , SceneObjectMaterials(sceneObject.SceneObjectMaterials)
        {}

    SceneObject& SceneObject::operator=(SceneObject& sceneObject) noexcept
    {
        Transform = sceneObject.Transform;
        SceneObjectModel = sceneObject.SceneObjectModel;
        SceneObjectMaterials = sceneObject.SceneObjectMaterials;

        return *this;
    }

    SceneObject::SceneObject(SceneObject&& sceneObject) noexcept
        : Transform(std::move(sceneObject.Transform))
        , SceneObjectModel(std::move(sceneObject.SceneObjectModel))
        , SceneObjectMaterials(std::move(sceneObject.SceneObjectMaterials))
        {}

    SceneObject& SceneObject::operator=(SceneObject&& sceneObject) noexcept
    {
        Transform = std::move(sceneObject.Transform);
        SceneObjectModel = std::move(sceneObject.SceneObjectModel);
        SceneObjectMaterials = std::move(sceneObject.SceneObjectMaterials);

        return *this;
    }

    SceneObject::SceneObject(Model* model, std::vector<Material*>& materials)
        : Transform(TransformQuat{})
    {
        SceneObjectModel = model;
        
        // allocate correct number of materials
        SceneObjectMaterials.resize(SceneObjectModel->Meshes.size(), nullptr);

        // fill vectors with materials
        for (unsigned int i = 0; i < materials.size(); ++i)
        {
            SceneObjectMaterials[i] = materials[i];
        }

        if (model->Meshes.size() != materials.size())
            Log::Error("SceneObject", "Unmatch between materials and meshes number"); // TODO: warning
    }

    void SceneObject::SetMeshMaterialLink(unsigned int meshIndex, Material* material)
    {
        if (SceneObjectMaterials.size() <= meshIndex)
        {
            Log::Error("SceneObject", "Tried to assign a material to a mesh index out of range"); // TODO: warning

            return;
        }

        SceneObjectMaterials[meshIndex] = material;
    }

    Material* SceneObject::GetMaterial(unsigned int meshIndex)
    {
        if (SceneObjectMaterials.size() <= meshIndex)
        {
            Log::Error("SceneObject", "Tried to get a material with a mesh index out of range"); // TODO: warning

            return nullptr;
        }

        return SceneObjectMaterials[meshIndex];
    }

    std::vector<Material*> SceneObject::GetSceneObjectMaterials()
    {
        return SceneObjectMaterials;
    }

    void SceneObject::ClearMeshMaterialLink(unsigned int meshIndex)
    {
        if (SceneObjectMaterials.size() <= meshIndex)
        {
            Log::Error("SceneObject", "Tried to delete a material association with a mesh index out of range"); // TODO: warning

            return;
        }

        SceneObjectMaterials[meshIndex] = nullptr;
    }

    Model* SceneObject::GetSceneObjectModel()
    {
        return SceneObjectModel;
    }

    void SceneObject::ClearSceneObjectModel()
    {
        SceneObjectModel = nullptr;
    }
}
