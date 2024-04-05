
#include <ezengine/scene.h>
#include <ezengine/transform.h>

Scene::Scene()
    : MainCamera(new Camera()) // default camera
{
    this->MainCamera->Transform.SetPosition(glm::vec3(.0f, .0f, 4.0f));
}

void Scene::Draw()
{
    for (unsigned int i = 0; i < this->GameObjects.size(); ++i)
    {
        this->GameObjects[i]->Model->Draw();
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

    for (auto& obj : GameObjects)
    {
        delete obj;
    }
}
