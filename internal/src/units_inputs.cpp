#include <imgui/imgui_impl_sdl3.h>
#include <vector>
#include "units/units_inputs.h"
#include "units/units_app.h"
#include "units/units_debug.h"


struct InputsData {
	SDL_Event event{};

	enum InputBit : uint8_t {
		InputBit_RAW,
		InputBit_STATE,
		InputBit_UP,
		InputBit_DOWN,
	};

	struct Input_t {
		const SDL_Scancode scancode{};
		Units::Bitset<uint8_t> bitset;
	};

	std::vector<Input_t>   global_inputs{};
	std::vector<Input_t>    stage_inputs{};
	std::vector<Input_t> substage_inputs{};

	bool    stage_inputs_enabled{ false };
	bool substage_inputs_enabled{ false };
} static InputsData{};


void Units::InputsManager::Init() noexcept {
	SDL_zero( InputsData.event );
}

void Units::InputsManager::EnableStageInputs   ( const bool& value ) noexcept {
	InputsData.   stage_inputs_enabled = value;
}
void Units::InputsManager::EnableSubStageInputs( const bool& value ) noexcept {
	InputsData.substage_inputs_enabled = value;
}
UNITS_NODISCARD const bool& Units::InputsManager::   StageInputsEnabled() noexcept {
	return InputsData.   stage_inputs_enabled;
}
UNITS_NODISCARD const bool& Units::InputsManager::SubStageInputsEnabled() noexcept {
	return InputsData.substage_inputs_enabled;
}

void Units::InputsManager::ClearGlobalInputs  () noexcept {
	InputsData.  global_inputs.clear();
}
void Units::InputsManager::ClearStageInputs   () noexcept {
	InputsData.   stage_inputs.clear();
}
void Units::InputsManager::ClearSubStageInputs() noexcept {
	InputsData.substage_inputs.clear();
}

UNITS_NODISCARD Units::Input_t Units::InputsManager::RegisterGlobalInput  ( const SDL_Scancode& scancode ) noexcept {
	Input_t input{
		static_cast<void*>( &InputsData.  global_inputs ),
		(uint16_t)InputsData.  global_inputs.size()
	};
	InputsData.  global_inputs.emplace_back( InputsData::Input_t{
		.scancode = scancode,
		.bitset = 0u,
	} );
	return input;
}
UNITS_NODISCARD Units::Input_t Units::InputsManager::RegisterStageInput   ( const SDL_Scancode& scancode ) noexcept {
	Input_t input{
		static_cast<void*>( &InputsData.   stage_inputs ),
		(uint16_t)InputsData.   stage_inputs.size()
	};
	InputsData.   stage_inputs.emplace_back( InputsData::Input_t{
		.scancode = scancode,
		.bitset = 0u,
	} );
	return input;
}
UNITS_NODISCARD Units::Input_t Units::InputsManager::RegisterSubStageInput( const SDL_Scancode& scancode ) noexcept {
	Input_t input{
		static_cast<void*>( &InputsData.substage_inputs ),
		(uint16_t)InputsData.substage_inputs.size()
	};
	InputsData.substage_inputs.emplace_back( InputsData::Input_t{
		.scancode = scancode,
		.bitset = 0u,
	} );
	return input;
}

UNITS_NODISCARD const bool Units::InputsManager::GetInputState( const Input_t& input ) noexcept {
	const std::vector<InputsData::Input_t>& registered_vector{ *static_cast<const std::vector<InputsData::Input_t>*>( input.registered_vector_ptr ) };
	return registered_vector[input.input_id].bitset.get( InputsData::InputBit_STATE );
}
UNITS_NODISCARD const bool Units::InputsManager::GetInputUp   ( const Input_t& input ) noexcept {
	const std::vector<InputsData::Input_t>& registered_vector{ *static_cast<const std::vector<InputsData::Input_t>*>( input.registered_vector_ptr ) };
	return registered_vector[input.input_id].bitset.get( InputsData::InputBit_UP    );
}
UNITS_NODISCARD const bool Units::InputsManager::GetInputDown ( const Input_t& input ) noexcept {
	const std::vector<InputsData::Input_t>& registered_vector{ *static_cast<const std::vector<InputsData::Input_t>*>( input.registered_vector_ptr ) };
	return registered_vector[input.input_id].bitset.get( InputsData::InputBit_DOWN  );
}

void Units::App::PollEvents() {
	while( SDL_PollEvent( &InputsData.event ) ) {
		ImGui_ImplSDL3_ProcessEvent( &InputsData.event );
		if( InputsData.event.type == SDL_EVENT_QUIT ) {
			Units::App::Exit();
			break;
		}

		if( InputsData.event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED ) {
			if( InputsData.event.window.windowID != SDL_GetWindowID( Window::GetMainWindowPtr() ) )
				Window::DestroyWindow( SDL_GetWindowFromID( InputsData.event.window.windowID ) );
			else {
				Units::App::Exit();
				break;
			}
		}

		if( InputsData.event.type == SDL_EVENT_KEY_UP || InputsData.event.type == SDL_EVENT_KEY_DOWN ) {
			for( InputsData::Input_t& input : InputsData.  global_inputs )
				if( InputsData.event.key.scancode == input.scancode ) {
					if( InputsData.event.type == SDL_EVENT_KEY_UP   )
						input.bitset.set( InputsData::InputBit_RAW, false );
					if( InputsData.event.type == SDL_EVENT_KEY_DOWN )
						input.bitset.set( InputsData::InputBit_RAW, true  );
				}
			if( InputsData.   stage_inputs_enabled )
				for( InputsData::Input_t& input : InputsData.   stage_inputs )
					if( InputsData.event.key.scancode == input.scancode ) {
						if( InputsData.event.type == SDL_EVENT_KEY_UP   )
							input.bitset.set( InputsData::InputBit_RAW, false );
						if( InputsData.event.type == SDL_EVENT_KEY_DOWN )
							input.bitset.set( InputsData::InputBit_RAW, true  );
					}
			if( InputsData.substage_inputs_enabled )
				for( InputsData::Input_t& input : InputsData.substage_inputs )
					if( InputsData.event.key.scancode == input.scancode ) {
						if( InputsData.event.type == SDL_EVENT_KEY_UP   )
							input.bitset.set( InputsData::InputBit_RAW, false );
						if( InputsData.event.type == SDL_EVENT_KEY_DOWN )
							input.bitset.set( InputsData::InputBit_RAW, true  );
					}
		}
	}
}

void SetInput( InputsData::Input_t& input ) {
	const bool raw  { input.bitset.get( InputsData::InputBit_RAW   ) };
	const bool state{ input.bitset.get( InputsData::InputBit_STATE ) };
	input.bitset.set( InputsData::InputBit_UP   , !raw &&  state );
	input.bitset.set( InputsData::InputBit_DOWN ,  raw && !state );
	input.bitset.set( InputsData::InputBit_STATE,  raw );
}
void Units::InputsManager::PollInputs() noexcept {
	for( InputsData::Input_t& input : InputsData.  global_inputs )
		SetInput( input );
	for( InputsData::Input_t& input : InputsData.   stage_inputs )
		SetInput( input );
	for( InputsData::Input_t& input : InputsData.substage_inputs )
		SetInput( input );
}
