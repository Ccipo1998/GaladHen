
#include "SystemsCoordinator.h"
#include <Systems/RenderingSystem/RenderingSystem.h>
#include <Systems/InputSystem/InputSystem.h>
#include <Systems/AssetSystem/AssetSystem.h>

namespace GaladHen
{
	void SystemsCoordinator::InitSystems()
	{
		if (Initialized)
			return;

		// Create weak singleton systems and init them
		AssetSys = new AssetSystem{};
		RenderingSys = new RenderingSystem{};
		InputSys = new InputSystem{};
		AssetSys->Init();
		RenderingSys->Init();
		InputSys->Init();

		Initialized = true;
	}

	void SystemsCoordinator::QuitSystems()
	{
		if (!Initialized)
			return;

		// Quit systems and free
		InputSys->Quit();
		RenderingSys->Quit();
		AssetSys->Quit();
		delete InputSys;
		delete RenderingSys;
		delete AssetSys;
	}

	SystemsCoordinator::SystemsCoordinator()
		: RenderingSys(nullptr)
		, InputSys(nullptr)
		, AssetSys(nullptr)
		, Initialized(false)
	{}
}
