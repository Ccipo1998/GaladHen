/*
Camera class

The camera class could have only few parameters, because many can be calculated starting from the basic ones (ex: view matrix from position, front and up directions;
    position, front and up directions from rotations around world X, Y and Z axis), but here all the data are explicitly saved in memory as an optimization (to avoid to
    perform the same calculations each time, especially for view and projection matrices)
*/

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <GLFW/glfw3.h>
#include <glm.hpp>
#include <gtx/transform.hpp> // for lookat() and perspective()
#include <gtx/quaternion.hpp>
#include <ext/quaternion_trigonometric.hpp>
#include <ext/quaternion_float.hpp>

#include <ezengine/transform.h>

class Camera
{
public:

    TransformQuat Transform;

    // movement parameters
    float LinearSpeed = 2.5f;
    float AngularSpeed = 100.0f;
    float MaxPitchAngle = 80.0f;

    // @brief
    // Default constructor
    Camera(const TransformQuat& transform = TransformQuat{}, float fovy = 45.0f, float aspectRatio = 1.7f, float nearDistance = 0.1f, float farDistance = 100.0f);

    // @brief
    // Move and rotate camera basing on inputs
    // @param keys: keyboard input keys status
    // @param mouse_keys: mouse input keys status
    // @param deltaX: delta x mouse coordinate
    // @param deltaY: delta y mouse coordinate
    // @param deltaTime: application delta time
    void ApplyMovements(const bool* keys, const bool* mouse_keys, const double& deltaX, const double& deltaY, float deltaTime);

    // setters

    void SetFovY(float fovy);
    void SetAspectRatio(float aspect);
    void SetNear(float nearDistance);
    void SetFar(float farDistance);

    // getters

    // @brief
    // Get view matrix: it's the transform's rotation converted to a matrix
    glm::mat4 GetViewMatrix();

    // @brief
    // Get projection matrix
    glm::mat4 GetProjectionMatrix();

    float GetFovY();
    float GetAspectRatio();
    float GetNear();
    float GetFar();

private:

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
