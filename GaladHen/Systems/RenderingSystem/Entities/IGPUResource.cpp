
#include "IGPUResource.h"

namespace GaladHen
{
	IGPUResource::IGPUResource()
		: ResourceID(0)
		, ResourceInvalidated(false)
	{}

	void IGPUResource::InvalidateResource()
	{
		ResourceInvalidated = true;
	}

	bool IGPUResource::IsResourceValid() const
	{
		return !ResourceInvalidated;
	}
}
