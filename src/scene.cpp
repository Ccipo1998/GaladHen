
#include <ezengine/scene.h>
#include <ezengine/camera.h>
#include <ezengine/sceneobject.h>
#include <ezengine/transform.h>
#include <ezengine/light.h>

Scene::Scene()
    : MainCamera(new Camera()) // default camera
{
    this->MainCamera->Transform.SetPosition(glm::vec3(.0f, .0f, 4.0f));
}

void Scene::Draw()
{
    for (unsigned int i = 0; i < this->SceneObjects.size(); ++i)
    {
        this->SceneObjects[i]->Draw();
    }
}

Scene::~Scene()
{
    // free memory

    delete MainCamera;

    for (auto& light : PointLights)
    {
        delete light;
    }

    for (auto& light : DirectionalLights)
    {
        delete light;
    }

    for (auto& obj : SceneObjects)
    {
        delete obj;
    }
}
