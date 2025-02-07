
// Entry point for data and functionalities of the Editor linking GaladHen libraries and features

#pragma once

#include <Systems/RenderingSystem/Entities/Scene.h>
#include <Systems/RenderingSystem/Common.h>
#include <Systems/RenderingSystem/RenderingSystem.h>

#include <vector>

// TODO: make editor reloadable: delete all current resources and reset to initial state -> this requires all the internal resources to have implemented their FreeResources() (or destructors) fuctionalities

namespace GaladHen
{
	class UIPage;

	class Editor final
	{

	public:

		// @brief
		// Init editor resources
		static void Init(API apiToUse = API::OpenGL);

		static void AddDefaultBunnyToScene();

		static void AddDefaultGizmosToScene();

		// @brief
		// Run the editor, assuming Init() was already called
		static void Run();

	private:

		static Scene Scene;

	};
}
