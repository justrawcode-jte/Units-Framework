#pragma once
#include <SDL3/SDL.h>
#include "./units_macros.h"
#include "./units_types.h"

namespace Units {
	/* ### \- Units Window_t opaque window pointer */
	typedef void Window_t;
	/* ### \- Units WindowID_t window id type */
	typedef uint32_t WindowID_t;
	/* ### \- Units WindowFlags_t window flags */
	typedef uint64_t WindowFlags_t;

	/* ### \- Units WindowCreateInfo_t window create info */
	struct WindowCreateInfo_t {
		const char* title{ nullptr };
		const WindowFlags_t window_flags{ 0u };
		const uint32_t width { 0u }; 
		const uint32_t height{ 0u }; 
	};

	namespace Window {
		extern void Init() noexcept;
		extern void Exit() noexcept;

		/* Creates Window_t
			Returns opaque pointer Window_t* */
		UNITS_NODISCARD extern Window_t* CreateWindow( const WindowCreateInfo_t& window_create_info ) noexcept;
		/* Destroys Window_t from opaque pointer */
		extern void DestroyWindow( Window_t* window_ptr ) noexcept;
		
		/* Gets main Window_t opaque pointer */
		UNITS_NODISCARD extern Window_t* GetMainWindowPtr() noexcept;
		/* Gets SDL's opaque pointer for Window_t equivalent */
		UNITS_NODISCARD extern SDL_Window* GetSDLWindowPtr( const Window_t* window_ptr ) noexcept;
		/* Destroys all created Window_t's */
		extern void DestroyAllWindows() noexcept;
		/* Destroys a Window_t by its WindowID_t */
		extern void DestroyWindowByID( const WindowID_t& window_id ) noexcept;
	} // namespace Window
	
} // namespace Core
