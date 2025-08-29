#pragma once
#include <SDL3/SDL_render.h>
#include "units/units_macros.h"
#include "units/units_types.h"
#include "units/units_window.h"

namespace Units {
	struct RendererCreateInfo_t {
		SDL_Window* window_ptr{ nullptr };
		void(*destroy_callback_ptr)(){ nullptr };
		const uint32_t width{ 0u }, height{ 0u };
		const SDL_ScaleMode scale_mode{ SDL_SCALEMODE_INVALID };
	};

	namespace Renderer {
		extern void Init() noexcept;
		extern void Exit() noexcept;
		extern void SetMainScaleMode( const SDL_ScaleMode& scale_mode ) noexcept;

		UNITS_NODISCARD extern SDL_Renderer* CreateRenderer( const RendererCreateInfo_t& renderer_create_info ) noexcept;
		extern void DestroyRenderer( SDL_Renderer* renderer_ptr ) noexcept;

		UNITS_NODISCARD extern SDL_Renderer* GetMainRendererPtr() noexcept;

		extern void     ClearStageTexture( SDL_Renderer* renderer_ptr, const SDL_Color& color ) noexcept;
		extern void  ClearSubStageTexture( SDL_Renderer* renderer_ptr, const SDL_Color& color ) noexcept;
		extern void      BindStageTexture( SDL_Renderer* renderer_ptr ) noexcept;
		extern void   BindSubStageTexture( SDL_Renderer* renderer_ptr ) noexcept;
		extern void    RenderStageTexture( SDL_Renderer* renderer_ptr ) noexcept;
		extern void RenderSubStageTexture( SDL_Renderer* renderer_ptr ) noexcept;
	} // namespace Renderer
} // namespace Units
