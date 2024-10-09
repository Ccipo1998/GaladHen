
// Material class holding data and pointer to shader

#pragma once

#include <glm/glm.hpp>
#include <string>
#include <vector>

#include "MaterialData.h"
#include "TextureData.h"

namespace GaladHen
{
    class ShaderPipeline;

    class Material
    {
    public:

        Material();

        Material(ShaderPipeline* materialShader, ShadingMode shadingMode);

        // Default copy and move contructors and assignments
        Material(const Material& material) = default;
        Material& operator=(const Material& material) = default;
        Material(Material&& material) = default;
        Material& operator=(Material&& material) = default;

        MaterialData* Data; // ownership of material data
        ShaderPipeline* MaterialShader;

        ShadingMode MaterialShadingMode;
    };

    // Material data structures

    // physically-based material data
    struct PBRMaterialData : public MaterialData
    {
        PBRMaterialData();

        virtual std::vector<MaterialDataScalar> GetScalarData() override;
        virtual std::vector<MaterialDataVector4> GetVector4Data() override;
        virtual std::vector<MaterialDataTexture> GetTextureData() override;
        virtual std::vector<std::string> GetFunctions() override;

        glm::vec4 Diffuse;
        glm::vec4 Specular;
        float Metallic;
        float Roughness;
        TextureParameters DiffuseTexture;
        TextureParameters NormalTexture;
        TextureParameters MetallicTexture;
        TextureParameters RoughnessTexture;
    };

    // Blinn-Phong material data
    struct BPMaterialData : public MaterialData
    {
        BPMaterialData();

        virtual std::vector<MaterialDataScalar> GetScalarData() override;
        virtual std::vector<MaterialDataVector4> GetVector4Data() override;

        // colors for the diffuse term and for the dielectric reflection
        glm::vec4 DiffuseColor;
        // Phong shading model parameters
        glm::vec4 Ka; // ambient component
        glm::vec4 Kd; // diffuse component (WARNING: if Kd is 1, it means that with colors that saturate an rgb channel the diffuse will behave like specular)
        glm::vec4 Ks; // specular component
        float SpecularFallOff; // falloff parameter from perfect specular direction (usually value from 1 to 200)
    };

    // Unlit material data
    struct UnlitMaterialData : public MaterialData
    {
        UnlitMaterialData();

        virtual std::vector<MaterialDataVector4> GetVector4Data() override;
        virtual std::vector<MaterialDataTexture> GetTextureData() override;
        virtual std::vector<std::string> GetFunctions() override;

        glm::vec4 DiffuseColor;
        bool UseVertexColor;
        TextureParameters DiffuseTexture;
    };

    // Generic material data
    struct GenericMaterialData : public MaterialData
    {
        GenericMaterialData();

        virtual std::vector<MaterialDataScalar> GetScalarData() override;
        virtual std::vector<MaterialDataInteger> GetIntegerData() override;
        virtual std::vector<MaterialDataVector2> GetVector2Data() override;
        virtual std::vector<MaterialDataVector3> GetVector3Data() override;
        virtual std::vector<MaterialDataVector4> GetVector4Data() override;
        virtual std::vector<MaterialDataTexture> GetTextureData() override;
        virtual std::vector<std::string> GetFunctions() override;

        std::vector<MaterialDataScalar> ScalarDatas;
        std::vector<MaterialDataInteger> IntegerDatas;
        std::vector<MaterialDataVector2> Vector2Datas;
        std::vector<MaterialDataVector3> Vector3Datas;
        std::vector<MaterialDataVector4> Vector4Datas;
        std::vector<MaterialDataTexture> TextureDatas;
        std::vector<std::string> FunctionDatas;
    };
}
