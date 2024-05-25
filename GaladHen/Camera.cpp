
#include "Camera.h"

namespace GaladHen
{

    Camera::Camera()
        : Transform(TransformQuat{})
        , FovY(45.0f)
        , AspectRatio(1.7f)
        , Near(0.1f)
        , Far(100.0f) // defaults
    {
        this->UpdateProjectionMatrix();
    }

    Camera::Camera(const TransformQuat& transform, float fovy, float aspectRatio, float nearDistance, float farDistance)
        : Transform(transform)
        , FovY(fovy)
        , AspectRatio(aspectRatio)
        , Near(nearDistance)
        , Far(farDistance)
    {
        this->UpdateProjectionMatrix();
    }

    void Camera::ApplyMovements(const bool* keys, const bool* mouse_keys, const double& deltaX, const double& deltaY, float deltaTime)
    {
        // keys
        if (keys[GLFW_KEY_W])
            this->Transform.SetPosition(this->Transform.GetPosition() + this->Transform.GetFront() * this->LinearSpeed * deltaTime);
        if (keys[GLFW_KEY_S])
            this->Transform.SetPosition(this->Transform.GetPosition() - this->Transform.GetFront() * this->LinearSpeed * deltaTime);
        if (keys[GLFW_KEY_D])
            this->Transform.SetPosition(this->Transform.GetPosition() + this->Transform.GetRight() * this->LinearSpeed * deltaTime);
        if (keys[GLFW_KEY_A])
            this->Transform.SetPosition(this->Transform.GetPosition() - this->Transform.GetRight() * this->LinearSpeed * deltaTime);
        if (keys[GLFW_KEY_E])
            this->Transform.SetPosition(this->Transform.GetPosition() + TransformQuat::GetGlobalUp() * this->LinearSpeed * deltaTime);
        if (keys[GLFW_KEY_Q])
            this->Transform.SetPosition(this->Transform.GetPosition() - TransformQuat::GetGlobalUp() * this->LinearSpeed * deltaTime);

        // rotation
        if (mouse_keys[GLFW_MOUSE_BUTTON_RIGHT])
        {
            // calculate delta radians within limits
            float deltaYaw = -float(this->AngularSpeed * deltaTime * deltaX);
            float deltaPitch = -float(this->AngularSpeed * deltaTime * deltaY);

            // pitch with limits
            float newPitch = this->Transform.GetPitch() + deltaPitch;
            deltaPitch = glm::clamp(newPitch, -this->MaxPitchAngle, this->MaxPitchAngle) - this->Transform.GetPitch();

            // apply rotation
            this->Transform.Rotate(deltaPitch, deltaYaw, 0.0f);
        }
    }

    void Camera::SetFovY(float fovy)
    {
        this->FovY = fovy;

        this->UpdateProjectionMatrix();
    }

    void Camera::SetAspectRatio(float aspect)
    {
        this->AspectRatio = aspect;

        this->UpdateProjectionMatrix();
    }

    void Camera::SetNear(float nearDistance)
    {
        this->Near = nearDistance;

        this->UpdateProjectionMatrix();
    }

    void Camera::SetFar(float farDistance)
    {
        this->Far = farDistance;

        this->UpdateProjectionMatrix();
    }

    glm::mat4 Camera::GetViewMatrix()
    {
        return glm::lookAt(this->Transform.GetPosition(), this->Transform.GetPosition() + this->Transform.GetFront(), TransformQuat::GetGlobalUp());;
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
        this->ProjectionMatrix = glm::perspective(glm::radians(this->FovY), this->AspectRatio, this->Near, this->Far);
    }
}
