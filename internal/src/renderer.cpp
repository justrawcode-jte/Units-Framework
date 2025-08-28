#include <SDL3/SDL.h>
#include "units/units_list.h"
#include "units/renderer/renderer.h"
#include "units/units_app.h"


struct RendererData{
	struct Renderer_t {
		SDL_Renderer* renderer_ptr{ nullptr };
	};
	Units::Renderer_t* main_renderer_ptr{ nullptr };
	std::list<Renderer_t> renderers{};
} static RendererData{};


void Units::Renderer::Init() noexcept {
	RendererData.main_renderer_ptr = CreateRenderer( RendererCreateInfo_t{
		.window_t = Window::GetMainWindowPtr(),
	} );
}
void Units::Renderer::Exit() noexcept {
	DestroyRenderer( RendererData.main_renderer_ptr );
}

UNITS_NODISCARD Units::Renderer_t* Units::Renderer::CreateRenderer( const RendererCreateInfo_t& renderer_create_info ) noexcept {
	RendererData::Renderer_t& renderer = RendererData.renderers.emplace_back( RendererData::Renderer_t{
		.renderer_ptr = SDL_CreateRenderer( Window::GetSDLWindowPtr( renderer_create_info.window_t ), nullptr ),
	} );
	return static_cast<Renderer_t*>( &renderer );
}
void Units::Renderer::DestroyRenderer( Renderer_t* renderer_ptr ) noexcept {
	std::list<RendererData::Renderer_t>::iterator renderer_it{ list::GetListIteratorFromPointer( &RendererData.renderers, static_cast<RendererData::Renderer_t*>( renderer_ptr ) ) };
	SDL_DestroyRenderer( renderer_it->renderer_ptr );
	RendererData.renderers.erase( renderer_it );
}

UNITS_NODISCARD Units::Renderer_t* Units::Renderer::GetMainRenderer() noexcept {
	return RendererData.main_renderer_ptr;
}
UNITS_NODISCARD SDL_Renderer* Units::Renderer::GetSDLRenderer( Renderer_t* renderer_ptr ) noexcept {
	return static_cast<RendererData::Renderer_t*>( renderer_ptr )->renderer_ptr;
}