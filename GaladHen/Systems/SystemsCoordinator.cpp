
#include "SystemsCoordinator.h"
#include <Systems/RenderingSystem/RenderingSystem.h>
#include <Systems/InputSystem/InputSystem.h>
#include <Systems/AssetSystem/AssetSystem.h>
#include <Systems/TimeSystem/TimeSystem.h>

namespace GaladHen
{
	void SystemsCoordinator::InitSystems()
	{
		if (Initialized)
			return;

		// Create weak singleton systems and init them
		TimeSys = new TimeSystem{};
		AssetSys = new AssetSystem{};
		RenderingSys = new RenderingSystem{};
		InputSys = new InputSystem{};
		TimeSys->Init();
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
		TimeSys->Quit();
		delete InputSys;
		delete RenderingSys;
		delete AssetSys;
		delete TimeSys;
	}

	SystemsCoordinator::SystemsCoordinator()
		: RenderingSys(nullptr)
		, InputSys(nullptr)
		, AssetSys(nullptr)
		, TimeSys(nullptr)
		, Initialized(false)
	{}
}
