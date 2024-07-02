
#include "scene.h"
#include "camera.h"
#include "transform.h"

namespace GaladHen
{
    Scene::Scene()
        : MainCamera(Camera()) // default camera
    {
        MainCamera.Transform.SetPosition(glm::vec3(.0f, .0f, 4.0f));
    }
}
