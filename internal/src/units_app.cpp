#include <imgui/imgui.h>
#include <string>
#include "units/units_config.h"
#include "units/units.h"

struct AppData {
	Units::Window_t* main_window_ptr{ nullptr };

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

UNITS_NODISCARD Units::Window_t* Units::App::GetMainWindowPtr() noexcept {
	return AppData.main_window_ptr;
}

void Units::App::Init() {
	SDL_Init( SDL_INIT_EVENTS | SDL_INIT_VIDEO );

	AppData.main_window_ptr = CreateWindow( WindowCreateInfo_t{
		.title = ( (std::string)"MASTER " + APP_VERSION_STRING ).c_str(),
		.window_flags = SDL_WINDOW_RESIZABLE,
		.width = 1280u,
		.height = 720u,
	} );

	InputsManager::Init();

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO(); (void)io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    ImGui::StyleColorsDark();
}

void Units::App::Exit() noexcept {
	if( AppData.is_running == true ) {
		AppData.is_running = false;
		return;
	}
	DestroyAllWindows();
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