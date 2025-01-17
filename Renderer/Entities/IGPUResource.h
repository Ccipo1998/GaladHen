
// Generic GPU loadable resource

#pragma once

#include <Renderer/GPUResourceInspector.h>

namespace GaladHen
{
	class IGPUResource
	{
	public:

		IGPUResource();
		bool IsResourceValid();

	protected:

		friend class GPUResourceInspector;

		void InvalidateResource();

		unsigned int ResourceID; // ID = 0 -> no linkage to gpu resource
		bool ResourceInvalidated;
	};
}
