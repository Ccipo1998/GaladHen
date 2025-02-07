
#include "GPUResourceInspector.h"
#include <Systems/RenderingSystem/Entities/IGPUResource.h>

namespace GaladHen
{
	unsigned int GPUResourceInspector::GetResourceID(const IGPUResource* resource)
	{
		return resource->ResourceID;
	}

	void GPUResourceInspector::SetResourceID(IGPUResource* resource, unsigned int id)
	{
		resource->ResourceID = id;
	}

	void GPUResourceInspector::ValidateResource(IGPUResource* resource)
	{
		resource->ResourceInvalidated = false;
	}
}
