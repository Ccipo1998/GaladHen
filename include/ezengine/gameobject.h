
#pragma once

#include <ezengine/model.h>
#include <ezengine/material.h>
#include <ezengine/transform.h>

class GameObject
{

public:

    TransformQuat Transform;

    Model* Model;
    IMaterial* Material;

private:

};
