
#include "Camera.h"

#include <glm/gtx/transform.hpp> // for lookat() and perspective()
#include <glm/gtx/quaternion.hpp>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/ext/quaternion_float.hpp>

namespace GaladHen
{
    Camera::Camera()
        : Transform(GaladHen::Transform{})
        , FovY(45.0f)
        , AspectRatio(1.7f)
        , Near(0.1f)
        , Far(100.0f) // defaults
    {
        UpdateProjectionMatrix();
    }

    Camera::Camera(const GaladHen::Transform& transform, float fovy, float aspectRatio, float nearDistance, float farDistance)
        : Transform(transform)
        , FovY(fovy)
        , AspectRatio(aspectRatio)
        , Near(nearDistance)
        , Far(farDistance)
    {
        UpdateProjectionMatrix();
    }

    void Camera::SetFovY(float fovy)
    {
        FovY = fovy;

        UpdateProjectionMatrix();
    }

    void Camera::SetAspectRatio(float aspect)
    {
        AspectRatio = aspect;

        UpdateProjectionMatrix();
    }

    void Camera::SetNear(float nearDistance)
    {
        Near = nearDistance;

        UpdateProjectionMatrix();
    }

    void Camera::SetFar(float farDistance)
    {
        Far = farDistance;

        UpdateProjectionMatrix();
    }

    glm::mat4 Camera::GetViewMatrix() const
    {
        return glm::lookAt(Transform.GetPosition(), Transform.GetPosition() + Transform.GetFront(), Transform::GlobalUp);
    }

    glm::mat4 Camera::GetProjectionMatrix() const
    {
        return ProjectionMatrix;
    }

    float Camera::GetFovY()
    {
        return FovY;
    }

    float Camera::GetAspectRatio()
    {
        return AspectRatio;
    }

    float Camera::GetNear()
    {
        return Near;
    }

    float Camera::GetFar()
    {
        return Far;
    }

    void Camera::UpdateProjectionMatrix()
    {
        ProjectionMatrix = glm::perspective(glm::radians(FovY), AspectRatio, Near, Far);
    }
}

