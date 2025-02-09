
#include "Material.h"
#include "ShaderPipeline.h"

#include <utility>

namespace GaladHen
{
	Material::Material()
		: Pipeline(std::weak_ptr<ShaderPipeline>{})
	{}

	Material::Material(std::weak_ptr<ShaderPipeline> pipeline)
		: Pipeline(pipeline)
	{}

	void Material::SetPipeline(std::weak_ptr<ShaderPipeline> pipeline)
	{
		Pipeline = pipeline;
	}

	std::weak_ptr<ShaderPipeline> Material::GetPipeline()
	{
		return Pipeline;
	}
}
