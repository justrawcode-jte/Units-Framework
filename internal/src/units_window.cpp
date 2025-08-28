#include "units/units_config.h"
#include "units/units_list.h"
#include "units/units_window.h"

struct WindowData {
	struct Window_t {
		const Units::WindowFlags_t window_flags{ 0u };
		SDL_Window* window_ptr{ nullptr };
	};

	Units::Window_t* main_window_ptr{ nullptr };

	std::list<Window_t> windows{};
} static WindowData{};
 

void Units::Window::Init() noexcept {
	WindowData.main_window_ptr = CreateWindow( WindowCreateInfo_t{
		.title = ( (std::string)"MASTER " + APP_VERSION_STRING ).c_str(),
		.window_flags = SDL_WINDOW_RESIZABLE,
		.width = 1280u,
		.height = 720u,
	} );
}
void Units::Window::Exit() noexcept {
	DestroyWindow( WindowData.main_window_ptr );
	DestroyAllWindows();
}

UNITS_NODISCARD Units::Window_t* Units::Window::CreateWindow( const WindowCreateInfo_t& window_create_info ) noexcept {
	return static_cast<Window_t*>( &WindowData.windows.emplace_back( WindowData::Window_t{
		.window_flags = window_create_info.window_flags,
		.window_ptr = SDL_CreateWindow( window_create_info.title, window_create_info.width, window_create_info.height, window_create_info.window_flags ),
	} ) );
}

UNITS_NODISCARD Units::Window_t* Units::Window::GetMainWindowPtr() noexcept {
	return WindowData.main_window_ptr;
}
UNITS_NODISCARD SDL_Window* Units::Window::GetSDLWindowPtr( const Units::Window_t* window_ptr ) noexcept {
	if( window_ptr == nullptr )
		return nullptr;
	return static_cast<const WindowData::Window_t*>( window_ptr )->window_ptr;
}

void Units::Window::DestroyWindow( Window_t* window_ptr ) noexcept {
	std::list<WindowData::Window_t>::iterator window_it{ list::GetListIteratorFromPointer( &WindowData.windows, static_cast<WindowData::Window_t*>( window_ptr ) ) };
	SDL_DestroyWindow( window_it->window_ptr );
	WindowData.windows.erase( window_it );
}
void Units::Window::DestroyAllWindows() noexcept {
	for( WindowData::Window_t& window : WindowData.windows )
		SDL_DestroyWindow( window.window_ptr );
	WindowData.windows.clear();
}
void Units::Window::DestroyWindowByID( const WindowID_t& window_id ) noexcept {
	if( WindowData.windows.size() <= 1u )
		return;
	std::list<WindowData::Window_t>::iterator window_it{ std::find_if(
		WindowData.windows.begin(),
		WindowData.windows.end(),
		[window_id]( const WindowData::Window_t& item ){
			return SDL_GetWindowID( item.window_ptr ) == window_id;
		}
	) };
	if( window_it != WindowData.windows.end() ) {
		SDL_DestroyWindow( window_it->window_ptr );
		WindowData.windows.erase( window_it );
	}
}