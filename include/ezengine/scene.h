
#pragma once

#include <vector>

#include <ezengine/camera.h>
#include <ezengine/light.h>

class Scene
{

public:

    Scene();

    Camera MainCamera;
    std::vector<PointLight> PointLights;

private:



};
