
#pragma once

namespace GaladHen
{
    class IGPUResource;

    class GPUResourceInspector
    {
    public:

        static unsigned int GetResourceID(const IGPUResource* resource);

        static void SetResourceID(IGPUResource* resource, unsigned int id);

        static void ValidateResource(IGPUResource* resource);
    };
}
