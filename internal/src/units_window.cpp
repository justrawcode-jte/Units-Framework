#include <vector>
#include "units/units_config.h"
#include "units/units_window.h"

struct WindowData {
	SDL_Window* main_window_ptr{ nullptr };

	std::vector<SDL_Window*> window_ptrs{};
	std::vector<void(*)()> callback_ptrs{};
} static WindowData{};
 

void Units::Window::Init( const WindowCreateInfo_t& main_window_create_info ) noexcept {
	WindowData.main_window_ptr = CreateWindow( main_window_create_info );
}
void Units::Window::Exit() noexcept {
	DestroyWindow( WindowData.main_window_ptr );
}

UNITS_NODISCARD SDL_Window* Units::Window::CreateWindow( const WindowCreateInfo_t& window_create_info ) noexcept {
	SDL_Window* window_ptr{ SDL_CreateWindow( window_create_info.title, window_create_info.width, window_create_info.height, window_create_info.window_flags ) };
	if( window_create_info.destroy_callback_ptr != nullptr ) {
		WindowData.window_ptrs.emplace_back( window_ptr );
		WindowData.callback_ptrs.emplace_back( window_create_info.destroy_callback_ptr );
	}
	return window_ptr;
}
void Units::Window::DestroyWindow( SDL_Window* window_ptr ) noexcept {
	if( window_ptr == nullptr )
		return;
	for( uint32_t i{ 0u }; i < WindowData.window_ptrs.size(); i++ )
		if( WindowData.window_ptrs[i] == window_ptr ) {
			(*WindowData.callback_ptrs[i])();
			WindowData.window_ptrs.  erase( WindowData.window_ptrs.  begin() + i );
			WindowData.callback_ptrs.erase( WindowData.callback_ptrs.begin() + i );
			break;
		}
	SDL_DestroyWindow( window_ptr );
	window_ptr = nullptr;
}

UNITS_NODISCARD SDL_Window* Units::Window::GetMainWindowPtr() noexcept {
	return WindowData.main_window_ptr;
}