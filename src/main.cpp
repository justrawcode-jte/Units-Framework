#include <string>
#include "units/units.h"
#include "units/renderer/renderer.h"
#define UNITS_ENTRY_POINT
#include "units/units_entry.h"

void destroy_window() noexcept;

struct MainStage : Units::Stage_t {
public:
	friend void destroy_window() noexcept;
public:
	void EnableInputs() override {
		pause_input = Units::InputsManager::RegisterStageInput( SDL_SCANCODE_ESCAPE );
	}
	void Enter() override {
		window_ptr = Units::Window::CreateWindow( Units::WindowCreateInfo_t{
			.title = "New Window",
			.window_flags = SDL_WINDOW_RESIZABLE,
			.destroy_callback_ptr = &destroy_window,
			.width = 640u,
			.height = 360u,
		} );
		renderer_ptr = Units::Renderer::CreateRenderer( Units::RendererCreateInfo_t{
			.window_ptr = window_ptr,
			.destroy_callback_ptr = nullptr,
			.width  = 640u,
			.height = 360u,
			.scale_mode = SDL_SCALEMODE_PIXELART,
		} );
	}
	void RenderPass() override {
		SDL_SetRenderDrawColor( Units::Renderer::GetMainRendererPtr(), 0u, 0u, 0u, 255u );
		SDL_RenderClear( Units::Renderer::GetMainRendererPtr() );
		Units::Renderer::ClearStageTexture( Units::Renderer::GetMainRendererPtr(), SDL_Color{ .r = 30u, .g = 30u, .b = 45u, .a = 255u } );
		Units::Renderer::RenderStageTexture( Units::Renderer::GetMainRendererPtr() );
		SDL_RenderPresent( Units::Renderer::GetMainRendererPtr() );
		if( renderer_ptr != nullptr ) {
			SDL_SetRenderDrawColor( renderer_ptr, 0u, 0u, 0u, 255u );
			SDL_RenderClear( renderer_ptr );
			Units::Renderer::ClearStageTexture( renderer_ptr, SDL_Color{ .r = 30u, .g = 30u, .b = 45u, .a = 255u } );
			Units::Renderer::RenderStageTexture( renderer_ptr );
			SDL_RenderPresent( renderer_ptr );
		}
	}
	void Exit() override {
	}
private:
	Units::Input_t pause_input{};
	SDL_Window* window_ptr{ nullptr };
	SDL_Renderer* renderer_ptr{ nullptr };

	struct SubStage : Units::SubStage_t {
	} substage{};
} static MainStage{};

void destroy_window() noexcept {
	Units::Renderer::DestroyRenderer( MainStage.renderer_ptr );
	MainStage.renderer_ptr = nullptr;
	MainStage.window_ptr = nullptr;
}

void Init() {
	Units::App::Init( Units::WindowCreateInfo_t{
		.title = ( (std::string)"Units-Framework " + APP_VERSION_STRING ).c_str(),
		.window_flags = SDL_WINDOW_RESIZABLE,
		.width  = 1280u,
		.height =  720u,
	} );
	Units::StageManager::LoadNextStage( &MainStage );
	Units::Renderer::SetMainScaleMode( SDL_SCALEMODE_PIXELART );
}
void Exit() {
	Units::App::Exit();
}
