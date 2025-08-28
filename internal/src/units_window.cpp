#include <list>
#include "units/units_window.h"

struct WindowData {
	struct Window_t {
		const Units::WindowFlags_t window_flags{ 0u };
		SDL_Window* window_ptr{ nullptr };
	};

	std::list<Window_t> windows{};
} static WindowData{};
 
UNITS_NODISCARD Units::Window_t* Units::CreateWindow( const WindowCreateInfo_t& window_create_info ) {
	return static_cast<Window_t*>( &WindowData.windows.emplace_back( WindowData::Window_t{
		.window_flags = window_create_info.window_flags,
		.window_ptr = SDL_CreateWindow( window_create_info.title, window_create_info.width, window_create_info.height, window_create_info.window_flags ),
	} ) );
}
void Units::DestroyWindow( Window_t* window_ptr ) {
	WindowData::Window_t& window_data = *static_cast<WindowData::Window_t*>( window_ptr );
	SDL_DestroyWindow( window_data.window_ptr );
	window_data.window_ptr = nullptr;
}

UNITS_NODISCARD SDL_Window* Units::GetWindowPtr( const Units::Window_t* window_ptr ) {
	if( window_ptr == nullptr )
		return nullptr;
	return static_cast<const WindowData::Window_t*>( window_ptr )->window_ptr;
}
void Units::DestroyAllWindows() {
	for( WindowData::Window_t& window_data : WindowData.windows ) {
		if( window_data.window_ptr == nullptr )
			continue;
		SDL_DestroyWindow( window_data.window_ptr );
		window_data.window_ptr = nullptr;
	}
	WindowData.windows.clear();
}
void Units::DestroyWindowByID( const WindowID_t& window_id ) {
	for( std::list<WindowData::Window_t>::iterator window_it; window_it != WindowData.windows.end(); window_it++ )
		if( window_it->window_ptr != nullptr && SDL_GetWindowID( window_it->window_ptr ) == window_id ) {
			SDL_DestroyWindow( window_it->window_ptr );
			window_it->window_ptr = nullptr;
			WindowData.windows.erase( window_it );
			break;
		}
}