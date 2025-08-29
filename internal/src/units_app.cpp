#include <imgui/imgui.h>
#include <imgui/imgui_impl_sdl3.h>
#include "units/units_config.h"
#include "units/units.h"

struct AppData {
	bool is_running{ false };

	struct Time {
		const uint8_t max_ticks_per_frame{ 100u };
		uint64_t prev_ms    { null64_v };
		uint64_t curr_ms    { null64_v };
		uint64_t ms_dur     { null64_v };
		uint64_t ticks_per_s{ 60u };
		uint64_t ms_per_tick{ 1000u / ticks_per_s };
	} Time{};
} static AppData{};


#if   defined(UNITS_IMGUI_IMPLSDL_GPU3)
#	include <imgui/imgui_impl_sdlgpu3.h>
#	include <imgui/imgui_impl_sdlgpu3_shaders.h>
#elif defined(UNITS_IMGUI_IMPLSDL_RENDERER3)
#	include <imgui/imgui_impl_sdlrenderer3.h>
#	include "units/renderer/renderer.h"
#endif

void Units::App::Init( const WindowCreateInfo_t& main_window_create_info ) {
	SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO );

	Window::Init( main_window_create_info );

	InputsManager::Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();

#if   defined(UNITS_IMGUI_IMPLSDL_GPU3)
	ImGui_ImplSDL3_InitForSDLGPU( GetSDLWindowPtr( AppData.main_window_ptr ) );
	ImGui_ImplSDLGPU3_InitInfo init_info{};
	init_info.Device = GPUData.device.device_ptr;
	init_info.ColorTargetFormat = SDL_GetGPUSwapchainTextureFormat( GPUData.device.device_ptr, Core::GetWindowPtr( GPUData.window_ptr ) );
	init_info.MSAASamples = SDL_GPU_SAMPLECOUNT_1;
	ImGui_ImplSDLGPU3_Init( &init_info );
#elif defined(UNITS_IMGUI_IMPLSDL_RENDERER3)
	Renderer::Init();
	ImGui_ImplSDL3_InitForSDLRenderer( Window::GetMainWindowPtr(), Renderer::GetMainRendererPtr() );
	ImGui_ImplSDLRenderer3_Init( Renderer::GetMainRendererPtr() );
#elif defined(UNITS_IMGUI_IMPLSDL3_OTHER)
	ImGui_ImplSDL3_InitForOther( Window::GetMainWindowPtr() );
#endif
}

void Units::App::Exit() noexcept {
	if( AppData.is_running == true ) {
		AppData.is_running = false;
		return;
	}
#if   defined(UNITS_IMGUI_IMPLSDL_GPU3)
#elif defined(UNITS_IMGUI_IMPLSDL_RENDERER3)
	Renderer::Exit();
#elif defined(UNITS_IMGUI_IMPLSDL3_OTHER)
#endif
	Window::Exit();
}

void BeginTick() {
	uint64_t curr_ms = SDL_GetTicks();
	AppData.Time.ms_dur += curr_ms - AppData.Time.prev_ms;
	AppData.Time.prev_ms = curr_ms;
}

void Units::App::Run() {
	if( AppData.is_running )
		return;
	AppData.is_running = true;

	AppData.Time.curr_ms =
	AppData.Time.prev_ms =
	AppData.Time.ms_dur  = 0u;

	while( AppData.is_running ) {
		Stage_t* const current_stage_ptr = StageManager::AttemptGetNextStage();
		if( current_stage_ptr == nullptr )
			break;

		current_stage_ptr->EnableInputs();
		current_stage_ptr->Enter();

		while( AppData.is_running ) {
			BeginTick();
			PollEvents();
			InputsManager::PollInputs();

			current_stage_ptr->BeginTick();

			uint8_t tick_count = 0u;
			while( AppData.Time.ms_dur >= AppData.Time.ms_per_tick && tick_count < AppData.Time.max_ticks_per_frame ) {
				current_stage_ptr->Tick();

				AppData.Time.ms_dur -= AppData.Time.ms_per_tick;
				tick_count++;
			}

			current_stage_ptr->UITick();
			current_stage_ptr->RenderPass();

			if( AppData.is_running && StageManager::CurrentPaused() ) {
				current_stage_ptr->Pause();

				while( AppData.is_running && StageManager::CurrentPaused() ) {
					SubStage_t* const current_substage_ptr = StageManager::AttemptGetNextSubStage();
					if( current_substage_ptr == nullptr ) {
						StageManager::EndSubStage();
						break;
					}

					current_substage_ptr->EnableInputs();
					current_substage_ptr->Enter();

					while( AppData.is_running && StageManager::CurrentPaused() ) {
						BeginTick();
						PollEvents();
						InputsManager::PollInputs();

						current_substage_ptr->BeginTick();

						current_substage_ptr->UITick();
						current_substage_ptr->RenderPass();

						current_substage_ptr->EndTick();

						if( StageManager::NewSubStage() )
							break;
					}

					InputsManager::EnableSubStageInputs( false );
					current_substage_ptr->Exit();
				}
				current_stage_ptr->Unpause();
			}

			current_stage_ptr->EndTick();

			if( StageManager::NewStage() )
				break;
		}
		InputsManager::ClearStageInputs();

		current_stage_ptr->Exit();
	}

	return;
}