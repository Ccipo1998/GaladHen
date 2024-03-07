
#include <ezengine/transform.h>
#include <gtx/quaternion.hpp>
#include <ext/quaternion_trigonometric.hpp>
#include <ext/quaternion_float.hpp>

TransformQuat::TransformQuat()
    : Position(glm::vec3{})
    , Rotation(glm::quat(1.0f, 0.0f, 0.0f, 0.0f))
    , Pitch(0.0f)
    , Yaw(0.0f)
    , Roll(0.0f)
    {}

void TransformQuat::Rotate(float deltaPitch, float deltaYaw, float deltaRoll)
{
    this->Pitch += deltaPitch;
    this->Yaw += deltaYaw;
    this->Roll += deltaRoll;

    this->UpdateRotation();
}

glm::vec3 TransformQuat::GetGlobalFront()
{
    return GLOBAL_FRONT;
}

glm::vec3 TransformQuat::GetGlobalUp()
{
    return GLOBAL_UP;
}

glm::vec3 TransformQuat::GetGlobalRight()
{
    return GLOBAL_RIGHT;
}

glm::vec3 TransformQuat::GetFront()
{
    return this->Rotation * GLOBAL_FRONT;
}

glm::vec3 TransformQuat::GetUp()
{
    return this->Rotation * GLOBAL_UP;
}

glm::vec3 TransformQuat::GetRight()
{
    return this->Rotation * GLOBAL_RIGHT;
}

glm::vec3 TransformQuat::GetPosition()
{
    return this->Position;
}

glm::quat TransformQuat::GetRotation()
{
    return this->Rotation;
}

float TransformQuat::GetPitch()
{
    return this->Pitch;
}

float TransformQuat::GetYaw()
{
    return this->Yaw;
}

float TransformQuat::GetRoll()
{
    return this->Roll;
}

void TransformQuat::SetPosition(const glm::vec3& position)
{
    this->Position = position;
}

void TransformQuat::SetRotation(const glm::quat& rotation)
{
    this->Rotation = rotation;
    this->UpdateEulerAngles();
}

void TransformQuat::SetPitch(float angle)
{
    this->Pitch = glm::mod(angle, 359.9999999f);
    this->UpdateRotation();
}

void TransformQuat::SetYaw(float angle)
{
    this->Yaw = glm::mod(angle, 359.9999999f);
    this->UpdateRotation();
}

void TransformQuat::SetRoll(float angle)
{
    this->Roll = glm::mod(angle, 359.9999999f);
    this->UpdateRotation();
}

// privates

void TransformQuat::UpdateRotation()
{
    float halfPitch = glm::radians(this->Pitch) * 0.5f;
    float sinPitch = glm::sin(halfPitch);
    float cosPitch = glm::cos(halfPitch);

    float halfYaw = glm::radians(this->Yaw) * 0.5f;
    float sinYaw = glm::sin(halfYaw);
    float cosYaw = glm::cos(halfYaw);

    float halfRoll = glm::radians(this->Roll) * 0.5f;
    float sinRoll = glm::sin(halfRoll);
    float cosRoll = glm::cos(halfRoll);

    this->Rotation.x = ((cosYaw * sinPitch) * cosRoll) + ((sinYaw * cosPitch) * sinRoll);
    this->Rotation.y = ((sinYaw * cosPitch) * cosRoll) - ((cosYaw * sinPitch) * sinRoll);
    this->Rotation.z = ((cosYaw * cosPitch) * sinRoll) - ((sinYaw * sinPitch) * cosRoll);
    this->Rotation.w = ((cosYaw * cosPitch) * cosRoll) + ((sinYaw * sinPitch) * sinRoll);
}

void TransformQuat::UpdateEulerAngles()
{
    glm::vec3 eulers = glm::eulerAngles(this->Rotation);
    this->Pitch = eulers.x;
    this->Yaw = eulers.y;
    this->Roll = eulers.z;
}
