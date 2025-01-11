
#include "Material.h"
#include "ShaderPipeline.h"

#include <utility>

namespace GaladHen
{
	Material::Material(std::shared_ptr<ShaderPipeline> pipeline)
		: Pipeline(pipeline)
	{}

	std::shared_ptr<ShaderPipeline> Material::GetPipeline()
	{
		return Pipeline;
	}
}
