
#include "GPUResourceInspector.h"
#include <Systems/RenderingSystem/Entities/IGPUResource.h>

namespace GaladHen
{
	unsigned int GPUResourceInspector::GetResourceID(const IGPUResource* resource)
	{
		if (!resource)
			return 0;

		return resource->ResourceID;
	}

	void GPUResourceInspector::SetResourceID(IGPUResource* resource, unsigned int id)
	{
		if (resource)
		{
			resource->ResourceID = id;
		}
	}

	void GPUResourceInspector::ValidateResource(IGPUResource* resource)
	{
		if (resource)
		{
			resource->ResourceInvalidated = false;
		}
	}
}
