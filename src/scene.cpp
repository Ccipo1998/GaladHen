
#include <ezengine/scene.h>

Scene::Scene()
    : MainCamera(Camera(vec3(.0f, .0f, 15.0f), radians(45.0f), 600.0f/400.0f, .1f, 100.0f)) // default camera
{
    this->MainCamera.setYaw(90.0f);
}

void Scene::Draw()
{
    for (unsigned int i = 0; i < this->GameObjects.size(); ++i)
    {
        this->GameObjects[i]->Model->draw();
    }
}
