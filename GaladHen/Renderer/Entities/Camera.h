
// The camera class could have only few parameters, because many can be calculated starting from the basic ones (ex: view matrix from position, front and up directions;
//     position, front and up directions from rotations around world X, Y and Z axis), but here all the data are explicitly saved in memory as an optimization (to avoid to
//     perform the same calculations each time, especially for view and projection matrices)

#pragma once

#include <glm/glm.hpp>

#include <Math/Transform.h>

namespace GaladHen
{
    class Camera
    {
    public:

        Camera();

        Camera(const GaladHen::Transform& transform, float fovy, float aspectRatio, float nearDistance, float farDistance);

        // Copy constructor, Copy assignment, Move constructor and Move assignment are defaulted

        // setters

        void SetFovY(float fovy);
        void SetAspectRatio(float aspect);
        void SetNear(float nearDistance);
        void SetFar(float farDistance);

        // getters

        glm::mat4 GetViewMatrix() const; // the transform is used to calculate the view matrix
        glm::mat4 GetProjectionMatrix() const;

        float GetFovY();
        float GetAspectRatio();
        float GetNear();
        float GetFar();

        Transform Transform;

    protected:

        // these data are private because there are specific function to handle the changes (because the changes could interest some other data)

        float FovY;
        float AspectRatio;
        float Near;
        float Far;

        glm::mat4 ProjectionMatrix;

        // @brief
        // Calculate new projection matrix basing on perspective parameters
        void UpdateProjectionMatrix();
    };
}
