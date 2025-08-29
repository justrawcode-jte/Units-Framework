#include <SDL3/SDL.h>
#include <vector>
#include "units/units_types.h"
#include "units/renderer/renderer.h"
#include "units/units_app.h"
#include "units/units_debug.h"


struct RendererData{
	struct Renderer_t {
		void(*destroy_callback_ptr)(){ nullptr };
		SDL_Texture*    stage_texture_ptr{ nullptr };
		SDL_Texture* substage_texture_ptr{ nullptr };
	} null_renderer{};

	SDL_Renderer* main_renderer_ptr{ nullptr };

	std::vector<SDL_Renderer*> renderer_ptrs{};
	std::vector<Renderer_t> renderers{};
} static RendererData{};

inline static RendererData::Renderer_t& get_renderer( SDL_Renderer* renderer_ptr ) noexcept {
	for( uint32_t i{ 0u }; i < RendererData.renderer_ptrs.size(); i++ )
		if( RendererData.renderer_ptrs[i] == renderer_ptr )
			return RendererData.renderers[i];
	return RendererData.null_renderer;
}


void Units::Renderer::Init() noexcept {
	RendererData.main_renderer_ptr = CreateRenderer( RendererCreateInfo_t{
		.window_ptr = Window::GetMainWindowPtr(),
		.destroy_callback_ptr = nullptr,
		.width  = 1280u,
		.height =  720u,
	} );
}
void Units::Renderer::Exit() noexcept {
	DestroyRenderer( RendererData.main_renderer_ptr );
}
void Units::Renderer::SetMainScaleMode( const SDL_ScaleMode& scale_mode ) noexcept {
	RendererData::Renderer_t renderer = get_renderer( RendererData.main_renderer_ptr );
	SDL_SetTextureScaleMode( renderer.   stage_texture_ptr, scale_mode );
	SDL_SetTextureScaleMode( renderer.substage_texture_ptr, scale_mode );
}

UNITS_NODISCARD SDL_Renderer* Units::Renderer::CreateRenderer( const RendererCreateInfo_t& renderer_create_info ) noexcept {
	SDL_Renderer* renderer_ptr{ RendererData.renderer_ptrs.emplace_back( SDL_CreateRenderer( renderer_create_info.window_ptr, nullptr ) ) };
	RendererData::Renderer_t& renderer = RendererData.renderers.emplace_back( RendererData::Renderer_t{
		.destroy_callback_ptr = renderer_create_info.destroy_callback_ptr,
		.   stage_texture_ptr = SDL_CreateTexture( renderer_ptr, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, renderer_create_info.width, renderer_create_info.height ),
		.substage_texture_ptr = SDL_CreateTexture( renderer_ptr, SDL_PIXELFORMAT_RGBA32, SDL_TEXTUREACCESS_TARGET, renderer_create_info.width, renderer_create_info.height ),
	} );
	SDL_SetTextureScaleMode( renderer.   stage_texture_ptr, renderer_create_info.scale_mode );
	SDL_SetTextureScaleMode( renderer.substage_texture_ptr, renderer_create_info.scale_mode );
	return renderer_ptr;
}
void Units::Renderer::DestroyRenderer( SDL_Renderer* renderer_ptr ) noexcept {
	for( uint32_t i{ 0u }; i < RendererData.renderer_ptrs.size(); i++ )
		if( RendererData.renderer_ptrs[i] == renderer_ptr ) {
			RendererData.renderer_ptrs.erase( RendererData.renderer_ptrs.begin() + i );
			RendererData::Renderer_t renderer = *RendererData.renderers.erase( RendererData.renderers.begin() + i );
			if( renderer.destroy_callback_ptr != nullptr )
				renderer.destroy_callback_ptr();
			SDL_DestroyTexture( renderer.   stage_texture_ptr );
			SDL_DestroyTexture( renderer.substage_texture_ptr );
			SDL_DestroyRenderer( renderer_ptr );
			break;
		}
}

UNITS_NODISCARD SDL_Renderer* Units::Renderer::GetMainRendererPtr() noexcept {
	return RendererData.main_renderer_ptr;
}

void     Units::Renderer::ClearStageTexture( SDL_Renderer* renderer_ptr, const SDL_Color& color ) noexcept {
	RendererData::Renderer_t& renderer{ get_renderer( renderer_ptr ) };
	SDL_SetRenderDrawColor( renderer_ptr, color.r, color.g, color.b, color.a );
	SDL_SetRenderTarget( renderer_ptr, renderer.   stage_texture_ptr );
	SDL_RenderClear( renderer_ptr );
	SDL_SetRenderDrawColor( renderer_ptr, 0u, 0u, 0u, 255u );
	SDL_SetRenderTarget( renderer_ptr, nullptr );
}
void  Units::Renderer::ClearSubStageTexture( SDL_Renderer* renderer_ptr, const SDL_Color& color ) noexcept {
	RendererData::Renderer_t& renderer{ get_renderer( renderer_ptr ) };
	SDL_SetRenderDrawColor( renderer_ptr, color.r, color.g, color.b, color.a );
	SDL_SetRenderTarget( renderer_ptr, renderer.substage_texture_ptr );
	SDL_RenderClear( renderer_ptr );
	SDL_SetRenderDrawColor( renderer_ptr, 0u, 0u, 0u, 255u );
	SDL_SetRenderTarget( renderer_ptr, nullptr );
}
void      Units::Renderer::BindStageTexture( SDL_Renderer* renderer_ptr ) noexcept {
	RendererData::Renderer_t& renderer{ get_renderer( renderer_ptr ) };
	SDL_SetRenderTarget( renderer_ptr, renderer.   stage_texture_ptr );
}
void   Units::Renderer::BindSubStageTexture( SDL_Renderer* renderer_ptr ) noexcept {
	RendererData::Renderer_t& renderer{ get_renderer( renderer_ptr ) };
	SDL_SetRenderTarget( renderer_ptr, renderer.substage_texture_ptr );
}
void    Units::Renderer::RenderStageTexture( SDL_Renderer* renderer_ptr ) noexcept {
	RendererData::Renderer_t& renderer{ get_renderer( renderer_ptr ) };
	int32_t win_width{ 0u }, win_height{ 0u };
	SDL_GetWindowSize( SDL_GetRenderWindow( renderer_ptr ), &win_width, &win_height );
	float32_t res_width{ (float32_t)renderer.   stage_texture_ptr->w }, res_height{ (float32_t)renderer.   stage_texture_ptr->h };
	const float32_t scale{ SDL_min( (float32_t)win_width / res_width, (float32_t)win_height / res_height ) };
	const SDL_FRect dst_rect{
		.x = 0.5f * ( (float32_t)win_width  - res_width  * scale ),
		.y = 0.5f * ( (float32_t)win_height - res_height * scale ),
		.w = res_width  * scale,
		.h = res_height * scale,
	};
	SDL_SetRenderTarget( renderer_ptr, nullptr );
	SDL_RenderTexture( renderer_ptr, renderer.   stage_texture_ptr, nullptr, &dst_rect );
}
void Units::Renderer::RenderSubStageTexture( SDL_Renderer* renderer_ptr ) noexcept {
	RendererData::Renderer_t& renderer{ get_renderer( renderer_ptr ) };
	int32_t win_width{ 0u }, win_height{ 0u };
	SDL_GetWindowSize( SDL_GetRenderWindow( renderer_ptr ), &win_width, &win_height );
	float32_t res_width{ (float32_t)renderer.substage_texture_ptr->w }, res_height{ (float32_t)renderer.substage_texture_ptr->h };
	const float32_t scale{ SDL_min( (float32_t)win_width / res_width, (float32_t)win_height / res_height ) };
	const SDL_FRect dst_rect{
		.x = 0.5f * ( (float32_t)win_width  - res_width  * scale ),
		.y = 0.5f * ( (float32_t)win_height - res_height * scale ),
		.w = res_width  * scale,
		.h = res_height * scale,
	};
	SDL_SetRenderTarget( renderer_ptr, nullptr );
	SDL_RenderTexture( renderer_ptr, renderer.substage_texture_ptr, nullptr, &dst_rect );
}