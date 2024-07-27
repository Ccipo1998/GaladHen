
#include "Material.h"
#include "ShaderProgram.h"

namespace GaladHen
{
    Material::Material()
        : MaterialShader(nullptr)
        , MaterialShadingMode(ShadingMode::SmoothShading)
        {}

    Material::Material(ShaderPipeline* materialShader, ShadingMode shadingMode)
        : MaterialShader(materialShader)
        , MaterialShadingMode(shadingMode)
        {}

    PBRMaterialData:: PBRMaterialData()
        : DiffuseColor(glm::vec4(0.3f, 0.9f, 0.3f, 1.0f))
        , DiffuseTexture(TextureParameters{})
        , NormalMap(TextureParameters{})
        , Metallic(0.0f)
        , MetallicTexture(TextureParameters{})
        , Roughness(0.0f)
        , RoughnessTexture(TextureParameters{})
        {}

    std::vector<MaterialDataScalar> PBRMaterialData::GetScalarData()
    {
        return std::vector<MaterialDataScalar>
        { 
            MaterialDataScalar{ std::string{"Metallic"}, Metallic }, 
            MaterialDataScalar{ std::string{"Roughness"}, Roughness } 
        };
    }

    std::vector<MaterialDataVector2> PBRMaterialData::GetVector2Data()
    {
        return std::vector<MaterialDataVector2>{};
    }

    std::vector<MaterialDataVector3> PBRMaterialData::GetVector3Data()
    {
        return std::vector<MaterialDataVector3>{};
    }

    std::vector<MaterialDataVector4> PBRMaterialData::GetVector4Data()
    {
        return std::vector<MaterialDataVector4>
        { 
            MaterialDataVector4{ std::string{"DiffuseColor"}, DiffuseColor }
        };
    }

    std::vector<MaterialDataTexture> PBRMaterialData::GetTextureData()
    {
        return std::vector<MaterialDataTexture>
        { 
            MaterialDataTexture{ std::string{"DiffuseTexture"}, DiffuseTexture }, 
            MaterialDataTexture{ std::string{"NormalMap"}, NormalMap },
            MaterialDataTexture{ std::string{"MetallicTexture"}, MetallicTexture },
            MaterialDataTexture{ std::string{"RoughnessTexture"}, RoughnessTexture } 
        };
    }

    BPMaterialData::BPMaterialData()
        : DiffuseColor(glm::vec4(0.3f, 0.9f, 0.3f, 1.0f))
        , Ka(glm::vec4(.1f, .1f, .1f, 1.0f))
        , Kd(glm::vec4(.6f, .6f, .6f, 1.0f))
        , Ks(glm::vec4(.8f, .8f, .8f, 1.0f))
        {}

    std::vector<MaterialDataScalar> BPMaterialData::GetScalarData()
    {
        return std::vector<MaterialDataScalar>
        { 
            MaterialDataScalar{ std::string{"SpecularFallOff"}, SpecularFallOff }
        };
    }

    std::vector<MaterialDataVector2> BPMaterialData::GetVector2Data()
    {
        return std::vector<MaterialDataVector2>{};
    }

    std::vector<MaterialDataVector3> BPMaterialData::GetVector3Data()
    {
        return std::vector<MaterialDataVector3>{};
    }

    std::vector<MaterialDataVector4> BPMaterialData::GetVector4Data()
    {
        return std::vector<MaterialDataVector4>
        { 
            MaterialDataVector4{ std::string{"DiffuseColor"}, DiffuseColor },
            MaterialDataVector4{ std::string{"Ka"}, Ka },
            MaterialDataVector4{ std::string{"Kd"}, Kd },
            MaterialDataVector4{ std::string{"Ks"}, Ks }
        };
    }

    std::vector<MaterialDataTexture> BPMaterialData::GetTextureData()
    {
        return std::vector<MaterialDataTexture>{};
    }
}
