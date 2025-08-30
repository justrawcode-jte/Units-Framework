#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include <imgui/imgui_impl_sdlrenderer3.h>
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
		console_input = Units::InputsManager::RegisterStageInput( SDL_SCANCODE_F1 );
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
	void BeginTick() override {
		ImGui_ImplSDLRenderer3_NewFrame();
		ImGui_ImplSDL3_NewFrame();
		ImGui::NewFrame();
	}
	void UITick() override {
		if( Units::InputsManager::GetInputUp( pause_input ) ) {
			Units::Debug::Log( "Paused" );
			Units::StageManager::LoadNextSubStage( &substage );
		}
		if( Units::InputsManager::GetInputUp( console_input ) )
			show_console = !show_console;
		if( show_console )
			Units::Debug::ShowDebugLogConsole( &show_console );
	}
	void RenderPass() override {
		ImGui::Render();
		SDL_SetRenderDrawColor( Units::Renderer::GetMainRendererPtr(), 0u, 0u, 0u, 255u );
		SDL_RenderClear( Units::Renderer::GetMainRendererPtr() );
		Units::Renderer::ClearStageTexture( Units::Renderer::GetMainRendererPtr(), SDL_Color{ .r = 30u, .g = 30u, .b = 45u, .a = 255u } );
		Units::Renderer::RenderStageTexture( Units::Renderer::GetMainRendererPtr() );
		ImGui_ImplSDLRenderer3_RenderDrawData( ImGui::GetDrawData(), Units::Renderer::GetMainRendererPtr() );
		SDL_RenderPresent( Units::Renderer::GetMainRendererPtr() );
		if( renderer_ptr != nullptr ) {
			SDL_SetRenderDrawColor( renderer_ptr, 0u, 0u, 0u, 255u );
			SDL_RenderClear( renderer_ptr );
			Units::Renderer::ClearStageTexture( renderer_ptr, SDL_Color{ .r = 30u, .g = 30u, .b = 45u, .a = 255u } );
			Units::Renderer::RenderStageTexture( renderer_ptr );
			SDL_RenderPresent( renderer_ptr );
		}
	}
	void Unpause() override {
		Units::Debug::Log( "Renter MainStage" );
	}
	void Exit() override {
	}
private:
	Units::Input_t pause_input{};
	Units::Input_t console_input{};
	bool show_console{ false };
	SDL_Window* window_ptr{ nullptr };
	SDL_Renderer* renderer_ptr{ nullptr };

	struct SubStage : Units::SubStage_t {
		SubStage( MainStage* _main_stage ): main_stage( _main_stage ) {}
		void Enter() override{
			Units::Debug::Log( "Entered SubStage_t: %u", pause_count );
			pause_count += 1u;
		}
		void EnableInputs() override {
			unpause_input = Units::InputsManager::RegisterSubStageInput( SDL_SCANCODE_P );
		}
		void BeginTick() override {
			ImGui_ImplSDLRenderer3_NewFrame();
			ImGui_ImplSDL3_NewFrame();
			ImGui::NewFrame();
		}
		void UITick() override {
			if( Units::InputsManager::GetInputUp( unpause_input ) ) {
				Units::Debug::Log( "Unpause" );
				Units::StageManager::EndSubStage();
			}
			if( Units::InputsManager::GetInputUp( main_stage->console_input ) )
				main_stage->show_console = !main_stage->show_console;
			if( main_stage->show_console )
				Units::Debug::ShowDebugLogConsole( &main_stage->show_console );
		}
		void RenderPass() override {
			ImGui::Render();
			SDL_SetRenderDrawColor( Units::Renderer::GetMainRendererPtr(), 0u, 0u, 0u, 255u );
			SDL_RenderClear( Units::Renderer::GetMainRendererPtr() );
			Units::Renderer::ClearSubStageTexture( Units::Renderer::GetMainRendererPtr(), SDL_Color{ .r = 0u, .g = 0u, .b = 0u, .a = 50u } );
			Units::Renderer::RenderStageTexture( Units::Renderer::GetMainRendererPtr() );
			Units::Renderer::RenderSubStageTexture( Units::Renderer::GetMainRendererPtr() );
			ImGui_ImplSDLRenderer3_RenderDrawData( ImGui::GetDrawData(), Units::Renderer::GetMainRendererPtr() );
			SDL_RenderPresent( Units::Renderer::GetMainRendererPtr() );
			if( main_stage->renderer_ptr != nullptr ) {
				SDL_SetRenderDrawColor( main_stage->renderer_ptr, 0u, 0u, 0u, 255u );
				SDL_RenderClear( main_stage->renderer_ptr );
				Units::Renderer::ClearSubStageTexture( main_stage->renderer_ptr, SDL_Color{ .r = 0u, .g = 0u, .b = 0u, .a = 50u } );
				Units::Renderer::RenderStageTexture( main_stage->renderer_ptr );
				Units::Renderer::RenderSubStageTexture( main_stage->renderer_ptr );
				SDL_RenderPresent( main_stage->renderer_ptr );
			}
		}
	private:
		MainStage* main_stage;
		uint32_t pause_count{ 0u };
		Units::Input_t unpause_input{};
	} substage{ this };
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
