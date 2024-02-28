
#pragma once

#include <vector>

#include <ezengine/gameobject.h>
#include <ezengine/camera.h>
#include <ezengine/light.h>

class Scene
{

public:

    Scene();

    void Draw();

    Camera MainCamera;
    std::vector<PointLight> PointLights;
    std::vector<DirectionalLight> DirectionalLights;

    std::vector<GameObject> GameObjects;

private:



};
