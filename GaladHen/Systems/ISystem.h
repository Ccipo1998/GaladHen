
#pragma once

namespace GaladHen
{
	class ISystem
	{
	protected:

		// @brief
		// System initialization, to call at the beginning
		virtual void Init() = 0;

		// @brief
		// System quit, to call at the end
		virtual void Quit() = 0;
	};
}
