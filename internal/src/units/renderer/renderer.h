#pragma once
#include <SDL3/SDL_render.h>
#include "units/units_macros.h"
#include "units/units_types.h"
#include "units/units_window.h"

namespace Units {
	typedef void Renderer_t;
	struct RendererCreateInfo_t {
		const Window_t* window_t{ nullptr };
	};

	namespace Renderer {
		extern void Init() noexcept;
		extern void Exit() noexcept;

		UNITS_NODISCARD extern Renderer_t* CreateRenderer( const RendererCreateInfo_t& renderer_create_info ) noexcept;
		extern void DestroyRenderer( Renderer_t* renderer_ptr ) noexcept;

		UNITS_NODISCARD extern Renderer_t* GetMainRenderer() noexcept;
		UNITS_NODISCARD extern SDL_Renderer* GetSDLRenderer( Renderer_t* renderer_ptr ) noexcept;
	} // namespace Renderer
} // namespace Units
