
#include "TimeSystem.h"

//#include <iostream>

namespace GaladHen
{
	TimeSystem::TimeSystem()
		: StartTime(0.0f)
		, LastTime(0.0f)
		, Time(0.0f)
		, DeltaTime(0.0f)
	{}

	void TimeSystem::Start()
	{
		StartTime = std::clock();
		LastTime = StartTime;
	}

	void TimeSystem::NewFrame()
	{
		float newTime = std::clock();

		DeltaTime = (newTime - LastTime) / CLOCKS_PER_SEC;
		Time = (newTime - StartTime) / CLOCKS_PER_SEC;

		LastTime = newTime;
	}

	float TimeSystem::GetDeltaTime()
	{
		if (DeltaTime <= 0.001f)
			DeltaTime = 0.0083f; // 120hz

		//std::cout << DeltaTime << std::endl;

		return DeltaTime;
	}

	float TimeSystem::GetTotalTime()
	{
		return Time;
	}

	void TimeSystem::Init()
	{

	}

	void TimeSystem::Quit()
	{

	}
}
