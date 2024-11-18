
// Entry point for data and functionalities of the Editor linking GaladHen libraries and features

#pragma once

#include <Core/Scene.h>
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

		static const Renderer& GetEditorRenderer();

	private:

		static API EditorAPI;

		static Scene EditorScene;

		static Renderer EditorRenderer;
		static Window EditorWindow;

		static UIPage* EditorUIPage;

	};
}
