
#pragma once

#include <vector>

#include <ezengine/gameobject.h>
#include <ezengine/camera.h>
#include <ezengine/light.h>

class Scene
{

public:

    Camera* MainCamera;
    std::vector<PointLight*> PointLights;
    std::vector<DirectionalLight*> DirectionalLights;

    std::vector<GameObject*> GameObjects;

    // @brief
    // Default constructor
    Scene();

    // @brief
    // Draw call on all game objects
    void Draw();

    ~Scene();

private:



};
