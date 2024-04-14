/*
Light classes for different light types
*/

#pragma once

// gl3w MUST be included before any other OpenGL-related header
#include <GL/gl3w.h>

#include <glm.hpp>
#include <ezengine/transform.h>

// Copy constructor, Copy assignment, Move constructor and Move assignment are defaulted

class Light
{
public:

    TransformQuat Transform;
    glm::vec3 Color;
    float Intensity;

    Light();

    Light(const glm::vec3& color, float intensity);
 
    // @brief
    // Convert light data in a shader-friendly format, because passing *this to shader won't work
    virtual void* GetLightDataForShader();

    // @brief
    // Return light data size in shader-friendly format
    virtual std::size_t GetLightSizeForShader();

    virtual ~Light();

protected:

    struct LightShaderData
    {
        glm::vec3 Position;
        glm::vec3 Color;
        float Intensity;
    };

    LightShaderData* LightDataShaderFormat;

};

class DirectionalLight : public Light
{
public:

    DirectionalLight();

    DirectionalLight(const glm::vec3& color, float intensity, const glm::vec3& direction);
    
    // @brief
    // Convert directional light data in a shader-friendly format, because passing *this to shader won't work
    virtual void* GetLightDataForShader() override;

    // @brief
    // Return directional light data size in shader-friendly format
    virtual std::size_t GetLightSizeForShader() override;

    // @brief
    // Static version of GetLightSizeForShader
    static std::size_t GetDirLightSizeForShader();

    // @brief
    // Get the direction of the light (front versor)
    glm::vec3 GetDirection();

    // @brief
    // Set the direction of the light (front versor)
    void SetDirection(const glm::vec3& direction);

protected:

    struct DirectionalLightShaderData : public LightShaderData
    {
        glm::vec3 Direction;
    };

};

class PointLight : public Light
{
public:

    float FallOffDistance;
    // TODO: falloff function type (penso vada bene un enum da cui dipende la funzione usata negli shader)

    PointLight();

    PointLight(const glm::vec3& color, float intensity, const glm::vec3& position);

    // @brief
    // Convert point light data in a shader-friendly format, because passing *this to shader won't work
    virtual void* GetLightDataForShader() override;

    // @brief
    // Return point light data size in shader-friendly format
    virtual std::size_t GetLightSizeForShader() override;

    // @brief
    // Static version of GetLightSizeForShader
    static std::size_t GetPointLightSizeForShader();

protected:

    struct PointLightShaderData : public LightShaderData
    {
        float FallOffDistance;
    };

};

class SpotLight : public PointLight
{
    public:

    float PenumbraAngle;
    float FallOffAngle; // this is the angle from the penumbra angle and the complete shadow (UmbraAngle = PenumbraAngle + FallOffAngle)
    // TODO: directional falloff function type (penso vada bene un enum da cui dipende la funzione usata negli shader)

    SpotLight();

    SpotLight(const glm::vec3& color, float intensity, const glm::vec3& position, float penumbra_angle, float falloff_angle);
};
