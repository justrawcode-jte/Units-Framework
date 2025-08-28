#pragma once
#include "./units_macros.h"
#include "./units_window.h"

namespace Units {
	namespace App {
		/* Inits the applicaitons core */
		extern void Init();
		/* Runs the application */
		extern void Run();
		/* Polls applicaitons events
			- Do not explicitly call, already called elsewhere */
		extern void PollEvents();
		/* When called, application will exit after the next tick */
		extern void Exit() noexcept;
	} // namespace App
} // namespace Core
