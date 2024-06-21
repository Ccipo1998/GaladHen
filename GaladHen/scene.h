
#pragma once

#include <vector>

#include "Camera.h"
#include "SceneObject.h"
#include "Light.h"

namespace GaladHen
{
    class Scene
    {

    public:

        // @brief
        // Default constructor
        Scene();

        Camera MainCamera;

        // lights
        std::vector<PointLight> PointLights;
        std::vector<DirectionalLight> DirectionalLights;

        std::vector<SceneObject> SceneObjects;

    };
}
