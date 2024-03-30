
#include <ezengine/scene.h>
#include <ezengine/transform.h>

Scene::Scene()
    : MainCamera(Camera()) // default camera
{
    this->MainCamera.Transform.SetPosition(vec3(.0f, .0f, 4.0f));
}

void Scene::Draw()
{
    for (unsigned int i = 0; i < this->GameObjects.size(); ++i)
    {
        this->GameObjects[i].Model->draw();
    }
}
