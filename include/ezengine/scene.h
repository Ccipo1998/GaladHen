
#pragma once

#include <vector>

class Camera;
class PointLight;
class DirectionalLight;
class SceneObject;
class Shader;

class Scene
{

public:

    Camera* MainCamera;
    std::vector<PointLight*> PointLights;
    std::vector<DirectionalLight*> DirectionalLights;

    Shader* PBRShader;

    std::vector<SceneObject*> SceneObjects;
    //std::vector<>

    // @brief
    // Default constructor
    Scene();

    // @brief
    // Draw call on all game objects
    void Draw();

    ~Scene();

private:



};
