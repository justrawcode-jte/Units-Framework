#pragma once
#include <SDL3/SDL.h>
#include "./units_macros.h"
#include "./units_types.h"

namespace Units {
	/* ### \- Units WindowCreateInfo_t SDL_Window create info */
	struct WindowCreateInfo_t {
		const char* title{ nullptr };
		const SDL_WindowFlags window_flags{ 0u };
		void(*destroy_callback_ptr)(){ nullptr };
		const uint32_t width { 0u }; 
		const uint32_t height{ 0u }; 
	};

	namespace Window {
		extern void Init( const WindowCreateInfo_t& main_window_create_info ) noexcept;
		extern void Exit() noexcept;

		/* Creates SDL_Window
			Returns SDL_Window pointer */
		UNITS_NODISCARD extern SDL_Window* CreateWindow( const WindowCreateInfo_t& window_create_info ) noexcept;
		/* Destroys SDL_Window pointer */
		extern void DestroyWindow( SDL_Window* window_ptr ) noexcept;
		
		/* Gets main SDL_Window pointer */
		UNITS_NODISCARD extern SDL_Window* GetMainWindowPtr() noexcept;
	} // namespace Window
	
} // namespace Core
