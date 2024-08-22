
#include "Material.h"
#include "ShaderProgram.h"

namespace GaladHen
{
    Material::Material()
        : MaterialShader(nullptr)
        , MaterialShadingMode(ShadingMode::SmoothShading)
        , Data(nullptr)
        {}

    Material::Material(ShaderPipeline* materialShader, ShadingMode shadingMode)
        : MaterialShader(materialShader)
        , MaterialShadingMode(shadingMode)
        , Data(nullptr)
        {}

    PBRMaterialData:: PBRMaterialData()
        : Diffuse(glm::vec4(0.3f, 0.9f, 0.3f, 1.0f))
        , Specular(glm::vec4(0.0f, 0.0f, 0.0f, 1.0f))
        , Metallic(0.0f)
        , Roughness(0.0f)
        , DiffuseTexture(TextureParameters{})
        , NormalTexture(TextureParameters{})
        , MetallicTexture(TextureParameters{})
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

    std::vector<MaterialDataInteger> PBRMaterialData::GetIntegerData()
    {
        return std::vector<MaterialDataInteger>{};
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
            MaterialDataVector4{ std::string{"Diffuse"}, Diffuse }
        };
    }

    std::vector<MaterialDataTexture> PBRMaterialData::GetTextureData()
    {
        return std::vector<MaterialDataTexture>
        { 
            MaterialDataTexture{ std::string{"DiffuseTexture"}, DiffuseTexture }, 
            MaterialDataTexture{ std::string{"NormalTexture"}, NormalTexture },
            MaterialDataTexture{ std::string{"MetallicTexture"}, MetallicTexture },
            MaterialDataTexture{ std::string{"RoughnessTexture"}, RoughnessTexture } 
        };
    }

    std::vector<std::string> PBRMaterialData::GetFunctions()
    {
        return std::vector<std::string>
        {
            DiffuseTexture.TextureSource != nullptr ? std::string{ "DiffuseSampling" } : std::string{ "DiffuseConstant" },
            NormalTexture.TextureSource != nullptr ? std::string{ "NormalSampling" } : std::string{ "NormalInterpolated" },
            MetallicTexture.TextureSource != nullptr ? std::string{ "MetallicSampling" } : std::string{ "MetallicConstant" },
            RoughnessTexture.TextureSource != nullptr ? std::string{ "RoughnessSampling" } : std::string{ "RoughnessConstant" }
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

    UnlitMaterialData::UnlitMaterialData()
        : DiffuseColor(glm::vec4(0.3f, 0.9f, 0.3f, 1.0f))
        , DiffuseTexture(TextureParameters{})
    {}

    std::vector<MaterialDataScalar> UnlitMaterialData::GetScalarData()
    {
        return std::vector<MaterialDataScalar>{};
    }

    std::vector<MaterialDataInteger> UnlitMaterialData::GetIntegerData()
    {
        return std::vector<MaterialDataInteger>{};
    }

    std::vector<MaterialDataVector2> UnlitMaterialData::GetVector2Data()
    {
        return std::vector<MaterialDataVector2>{};
    }

    std::vector<MaterialDataVector3> UnlitMaterialData::GetVector3Data()
    {
        return std::vector<MaterialDataVector3>{};
    }

    std::vector<MaterialDataVector4> UnlitMaterialData::GetVector4Data()
    {
        return std::vector<MaterialDataVector4>
        {
            MaterialDataVector4{ std::string{"Diffuse"}, DiffuseColor }
        };
    }

    std::vector<MaterialDataTexture> UnlitMaterialData::GetTextureData()
    {
        return std::vector<MaterialDataTexture>
        {
            MaterialDataTexture{ std::string{"DiffuseTexture"}, DiffuseTexture }
        };
    }

    std::vector<std::string> UnlitMaterialData::GetFunctions()
    {
        return std::vector<std::string>
        {
            DiffuseTexture.TextureSource != nullptr ? std::string{ "DiffuseSampling" } : std::string{ "DiffuseConstant" }
        };
    }
}
