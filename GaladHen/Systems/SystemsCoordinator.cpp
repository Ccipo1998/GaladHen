
#include "SystemsCoordinator.h"
#include <Systems/RenderingSystem/RenderingSystem.h>
#include <Systems/InputSystem/InputSystem.h>

namespace GaladHen
{
	void SystemsCoordinator::InitSystems()
	{
		if (Initialized)
			return;

		// Create weak singleton systems and init them
		RenderingSys = new RenderingSystem{};
		InputSys = new InputSystem{};
		RenderingSys->Init();
		InputSys->Init();

		Initialized = true;
	}

	void SystemsCoordinator::QuitSystems()
	{
		// Quit systems and free
		InputSys->Quit();
		RenderingSys->Quit();
		delete InputSys;
		delete RenderingSys;
	}

	SystemsCoordinator::SystemsCoordinator()
		: RenderingSys(nullptr)
		, InputSys(nullptr)
		, Initialized(false)
	{}
}
