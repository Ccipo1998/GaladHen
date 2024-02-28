
#include <ezengine/transform.h>

TransformQuat::TransformQuat()
    : Position(glm::vec3{})
    , Rotation(glm::quat{ 1.0f, 0.0f, 0.0f, 0.0f})
    {}

glm::vec3 TransformQuat::GetFront()
{
    return GLOBAL_FRONT * this->Rotation;
}

glm::vec3 TransformQuat::GetUp()
{
    return GLOBAL_UP * this->Rotation;
}

glm::vec3 TransformQuat::GetRight()
{
    return GLOBAL_RIGHT * this->Rotation;
}
