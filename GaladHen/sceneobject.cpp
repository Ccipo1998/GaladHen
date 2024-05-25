
#include "sceneobject.h"
#include "model.h"
#include "pbrmaterial.h"
#include <utils/log.h>

SceneObject::SceneObject()
    : Transform(TransformQuat{})
    {}

SceneObject::SceneObject(const Model* model)
    : Transform(TransformQuat{})
{
    if (model == nullptr)
    {
        Log::Error("SceneObject", "Null model for scene object");

        return;
    }

    this->SceneObjectModel = model;
    
    // allocate correct number of materials
    this->SceneObjectMaterials.resize(this->SceneObjectModel->Meshes.size(), nullptr);
}

void SceneObject::SetMaterial(unsigned int meshIndex, Material* material)
{
    if (this->SceneObjectMaterials.size() <= meshIndex)
    {
        Log::Error("SceneObject", "Tried to assign a material to a mesh index out of range");

        return;
    }

    this->SceneObjectMaterials[meshIndex] = material;
}

Material* SceneObject::GetMaterial(unsigned int meshIndex)
{
    if (this->SceneObjectMaterials.size() <= meshIndex)
        return nullptr;

    return this->SceneObjectMaterials[meshIndex];
}

void SceneObject::Draw()
{
    if (this->SceneObjectModel == nullptr)
    {
        Log::Error("SceneObject", "Drawcall on a null scene object model");

        return;
    }

    this->SceneObjectModel->Draw(this->SceneObjectMaterials);
}
