
#pragma once

#include <Utils/singleton.hpp>

namespace GaladHen
{
	class RenderingSystem;
	class InputSystem;
	class AssetSystem;
	class TimeSystem;

	class SystemsCoordinator : public Singleton<SystemsCoordinator>
	{
		friend Singleton;

	public:

		void InitSystems();

		void QuitSystems();

	private:

		SystemsCoordinator();

		// Systems references
		RenderingSystem* RenderingSys;
		InputSystem* InputSys;
		AssetSystem* AssetSys;
		TimeSystem* TimeSys;

		// Status
		bool Initialized;
	};
}
