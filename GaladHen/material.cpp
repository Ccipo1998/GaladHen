
#include "Material.h"
#include "ShaderProgram.h"

namespace GaladHen
{
    Material::Material()
        : MaterialShader(nullptr)
        , MaterialShadingMode(ShadingMode::SmoothShading)
        {}

    Material::Material(ShaderProgram* materialShader, ShadingMode shadingMode)
        : MaterialShader(materialShader)
        , MaterialShadingMode(shadingMode)
        {}

    PBRMaterialData:: PBRMaterialData()
        : DiffuseColor(glm::vec3(0.3f, 0.9f, 0.3f))
        , DiffuseTexture(Texture{})
        , NormalMap(Texture{})
        , Metallic(0.0f)
        , MetallicTexture(Texture{})
        , Roughness(0.0f)
        , RoughnessTexture(Texture{})
        {}

    BPMaterialData::BPMaterialData()
        : DiffuseColor(glm::vec3(0.3f, 0.9f, 0.3f))
        , Ka(glm::vec3(.1f, .1f, .1f))
        , Kd(glm::vec3(.6f, .6f, .6f))
        , Ks(glm::vec3(.8f, .8f, .8f))
        {}
}
