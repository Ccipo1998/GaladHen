
// Entry point for data and functionalities of the Editor linking GaladHen libraries and features

#pragma once

#include <Renderer/Entities/Scene.h>
#include <Renderer/Common.h>
#include <Renderer/Renderer.h>
#include <Renderer/Window.h>

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

		static API API;
		static Scene Scene;
		static Window Window;

		static UIPage* EditorUIPage;

	};
}
