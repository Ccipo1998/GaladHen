
#include "scene.h"
#include "camera.h"
#include "transform.h"

namespace GaladHen
{
    Scene::Scene()
        : MainCamera(Camera()) // default camera
    {
        MainCamera.Transform.SetPosition(glm::vec3(.0f, .0f, 4.0f));

        // Default lighting
        PointLight pLight{};
        pLight.Transform.SetPosition(glm::vec3(0.0f, 0.0f, 5.0f));
        PointLights.emplace_back(pLight);
    }
}
