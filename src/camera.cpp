
#include <ezengine/camera.h>

Camera::Camera(const TransformQuat& transform, float fovy, float aspectRatio, float nearDistance, float farDistance)
    : Transform(transform)
    , Pitch(0.0f)
    , Yaw(0.0f)
    , Roll(0.0f)
    , FovY(fovy)
    , AspectRatio(aspectRatio)
    , Near(nearDistance)
    , Far(farDistance)
{
    UpdateViewMatrix();
    UpdateProjectionMatrix();
}

void Camera::ApplyMovements(const bool* keys, const bool* mouse_keys, const double& deltaX, const double& deltaY, float deltaTime)
{
    // keys
    if (keys[GLFW_KEY_W])
        SetPosition(this->Transform.Position + this->Transform.GetFront() * this->LinearSpeed * deltaTime);
    if(keys[GLFW_KEY_S])
        SetPosition(this->Transform.Position - this->Transform.GetFront() * this->LinearSpeed * deltaTime);
    if(keys[GLFW_KEY_D])
        SetPosition(this->Transform.Position + this->Transform.GetRight() * this->LinearSpeed * deltaTime);
    if(keys[GLFW_KEY_A])
        SetPosition(this->Transform.Position - this->Transform.GetRight() * this->LinearSpeed * deltaTime);

    // mouse position
    if (mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
    {
        // rotations

        // calculate delta radians within limits
        float deltaRadiansX = float(this->AngularSpeed * deltaTime * deltaX);
        float deltaRadiansY = float(this->AngularSpeed * deltaTime * deltaY);
        // pitch with limits
        float newPitch = this->Pitch + glm::degrees(deltaRadiansY);
        deltaRadiansY = glm::radians(glm::clamp(newPitch, -this->MaxPitchAngle, this->MaxPitchAngle) - this->Pitch);

        // calculate rotations -> only on front because inside the camera we don't want the roll
        glm::vec3 newFront = this->Transform.GetFront();
        glm::quat rotQuatX = glm::angleAxis(deltaRadiansX, glm::vec3(0.0f, 1.0f, 0.0f));
        newFront = newFront * rotQuatX;
        glm::quat rotQuatY = glm::angleAxis(deltaRadiansY, this->Transform.GetRight());
        newFront = newFront * rotQuatY;
        newFront = glm::normalize(newFront);

        // new lookat matrix quaternion
        glm::mat4 mat = glm::lookAt(this->Transform.Position, this->Transform.Position + newFront, glm::vec3(0.0f, 1.0f, 0.0f));
        this->Transform.Rotation = glm::toQuat(mat);

        // extrapolate euler angles
        glm::vec3 eulers = glm::eulerAngles(this->Transform.Rotation);
        this->Pitch = glm::degrees(eulers.x);
        this->Yaw = glm::degrees(eulers.y);
        this->Roll = glm::degrees(eulers.z);

        UpdateViewMatrix();
    }
}

void Camera::SetPosition(const glm::vec3& position)
{
    this->Transform.Position = position;

    UpdateViewMatrix();
}

void Camera::SetPitch(float pitch)
{
    pitch = fmod(pitch, 360.0f);
    glm::quat rotQuat = glm::angleAxis(glm::radians(pitch - this->Pitch), glm::vec3(1.0f, .0f, .0f));
    this->Front = rotQuat * this->Front;
    this->Up = rotQuat * this->Up;
    this->Right = glm::cross(this->Front, this->Up);
    this->Pitch = pitch;

    UpdateViewMatrix();
}

void Camera::SetYaw(float yaw)
{
    yaw = fmod(yaw, 360.0f);
    glm::quat rotQuat = glm::angleAxis(glm::radians(yaw - this->Yaw), glm::vec3(.0f, 1.0f, .0f));
    this->Front = rotQuat * this->Front;
    this->Up = rotQuat * this->Up;
    this->Right = glm::cross(this->Front, this->Up);
    this->Yaw = yaw;

    UpdateViewMatrix();
}

void Camera::SetRoll(float roll)
{
    roll = fmod(roll, 360.0f);
    glm::quat rotQuat = glm::angleAxis(glm::radians(roll - this->Roll), glm::vec3(.0f, .0f, 1.0f));
    this->Front = rotQuat * this->Front;
    this->Up = rotQuat * this->Up;
    this->Right = glm::cross(this->Front, this->Up);
    this->Roll = roll;

    UpdateViewMatrix();
}

void Camera::SetFovY(float fovy)
{
    this->FovY = fovy;

    UpdateProjectionMatrix();
}

void Camera::SetAspectRatio(float aspect)
{
    this->AspectRatio = aspect;

    UpdateProjectionMatrix();
}

void Camera::SetNear(float nearDistance)
{
    this->Near = nearDistance;

    UpdateProjectionMatrix();
}

void Camera::SetFar(float farDistance)
{
    this->Far = farDistance;

    UpdateProjectionMatrix();
}

glm::vec3 Camera::GetPosition()
{
    return this->Transform.Position;
}

float Camera::GetRoll()
{
    return this->Roll;
}

float Camera::GetPitch()
{
    return this->Pitch;
}

float Camera::GetYaw()
{
    return this->Yaw;
}

glm::mat4 Camera::GetViewMatrix()
{
    return this->ViewMatrix;
}

glm::mat4 Camera::GetProjectionMatrix()
{
    return this->ProjectionMatrix;
}

float Camera::GetFovY()
{
    return this->FovY;
}

float Camera::GetAspectRatio()
{
    return this->AspectRatio;
}

float Camera::GetNear()
{
    return this->Near;
}

float Camera::GetFar()
{
    return this->Far;
}

void Camera::UpdateProjectionMatrix()
{
    this->ProjectionMatrix = glm::perspective(this->FovY, this->AspectRatio, this->Near, this->Far);
}

void Camera::UpdateViewMatrix()
{
    this->ViewMatrix = glm::lookAt(this->Transform.Position, this->Transform.Position + this->Transform.GetFront(), glm::vec3(0.0f, 1.0f, 0.0f));
}
