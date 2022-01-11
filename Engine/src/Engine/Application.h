#pragma once

#include "LBD_Core.h"

namespace lbd {

	class LBD_API Application
	{
	public:
		Application();
		virtual ~Application();

		void run();
	};

	// Define this in each game.
	Application* CreateGame();;

}
