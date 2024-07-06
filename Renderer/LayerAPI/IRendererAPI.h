
// This is a pure virtual class (interface) to define basic functionalities of a renderer at API level

#pragma once

namespace GaladHen
{
	class IRendererAPI
	{
	public:

		virtual void CreateLowLevelMesh() = 0;

		virtual void DestroyLowLevelMesh() = 0;

	};
}
