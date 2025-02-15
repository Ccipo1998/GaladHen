
#pragma once

#include <Systems/ISystem.h>
#include <Utils/WeakSingleton.hpp>
#include <Systems/SystemsCoordinator.h>
#include <ctime>

namespace GaladHen
{
	class TimeSystem : public ISystem, public WeakSingleton<TimeSystem>
	{
		friend SystemsCoordinator;

	public:

		TimeSystem();

		// @brief
		// Start time
		void Start();

		// brief
		// Trigger new frame updates
		void NewFrame();

		// @brief
		// Get delta time of current frame
		float GetDeltaTime();

		// @brief
		// Get total time from the beginning
		float GetTotalTime();

	private:

		virtual void Init() override;
		virtual void Quit() override;

		clock_t StartTime;
		clock_t LastTime;
		float Time;
		float DeltaTime;

	};
}
