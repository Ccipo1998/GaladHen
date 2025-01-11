
#include "IGPUResource.h"

namespace GaladHen
{
	IGPUResource::IGPUResource()
		: ResourceID(0)
	{}

	void IGPUResource::InvalidateResource()
	{
		// ID is canceled -> we lost linkage to gpu resource
		ResourceID = 0;
	}
}
