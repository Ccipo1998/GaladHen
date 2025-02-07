
#include "Scene.h"

namespace GaladHen
{
    Scene::Scene()
        : MainCamera(Camera()) // default camera
    {
        MainCamera.Transform.SetPosition(glm::vec3(.0f, 1.0f, 4.0f));
        MainCamera.Transform.RotateYaw(90.0f);

        // Default lighting
        DirectionalLight dLight{};
        dLight.SetLightDirection(glm::vec3(-0.5f, -0.5f, -0.5f));
        DirectionalLights.emplace_back(dLight);

        /*PointLight pLight{};
        pLight.Transform.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
        PointLights.emplace_back(pLight);*/
    }
}
