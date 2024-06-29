
// Material class holding data and pointer to shader

#pragma once

#include <glm.hpp>
#include <string>
#include <vector>

#include <Common/MaterialData.h>
#include "Texture.h"

namespace GaladHen
{
    class ShaderProgram;
    struct MaterialData;

    class Material
    {
    public:

        Material();

        Material(ShaderProgram* materialShader, ShadingMode shadingMode);

        // Default copy and move contructors and assignments
        Material(const Material& material) = default;
        Material& operator=(const Material& material) = default;
        Material(Material&& material) = default;
        Material& operator=(Material&& material) = default;

        MaterialData* Data; // ownership of material data
        ShaderProgram* MaterialShader;

        ShadingMode MaterialShadingMode;
    };

    // Material data structures

    // physically-based material data
    struct PBRMaterialData : public MaterialData
    {
        PBRMaterialData();

        virtual std::vector<MaterialDataScalar> GetScalarData() override;
        virtual std::vector<MaterialDataVector> GetVectorData() override;
        virtual std::vector<MaterialDataTexture> GetTextureData() override;

        glm::vec3 DiffuseColor;
        Texture DiffuseTexture;
        Texture NormalMap;
        float Metallic;
        Texture MetallicTexture;
        float Roughness;
        Texture RoughnessTexture;
        //GLfloat Specular;
    };

    // Blinn-Phong material data
    struct BPMaterialData : public MaterialData
    {
        BPMaterialData();

        // colors for the diffuse term and for the dielectric reflection
        glm::vec3 DiffuseColor;
        // Phong shading model parameters
        glm::vec3 Ka; // ambient component
        glm::vec3 Kd; // diffuse component (WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular)
        glm::vec3 Ks; // specular component
        float SpecularFallOff; // falloff parameter from perfect specular direction (usually value from 1 to 200)
    };
}
