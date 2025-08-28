// #include <imgui/imgui_impl_sdl3.h>
/* #include "units/units_inputs.h"

struct InputsData {
	SDL_Event event;

    enum InputData : uint8_t {
        InputData_RAW,
        InputData_STATE,
        InputData_UP,
        InputData_DOWN,
    };

    struct Input {
        SDL_Scancode scancode;
        Units::Bitset<uint8_t> set;
    };

	std::vector<Input> internal_inputs;
	std::vector<Input>    stage_inputs;
	std::vector<Input> substage_inputs;
} static InputsData{};

Units::_Mouse Units::Mouse{};
Units::Bitset<uint8_t> Units::_Mouse::_l, Units::_Mouse::_m, Units::_Mouse::_r;


void Units::InitInputs() {
    SDL_zero( InputsData.event );
}

void Units::DisableStageInputs() noexcept {
    InputsData.stage_inputs.clear();
}
void Units::DisableSubStageInputs() noexcept {
    InputsData.substage_inputs.clear();
}

UNITS_NODISCARD Units::Input Units::InputsManager::RegisterInternalInput( const SDL_Scancode &_scancode ) noexcept {
    Input input;
    input.registered_vector = &InputsData.internal_inputs;
    input.input_index = InputsData.internal_inputs.size();
    InputsData.internal_inputs.emplace_back( _Input{
        .scancode = _scancode,
        .set = 0u,
    } );
    return input;
}
UNITS_NODISCARD Units::Input Units::InputsManager::RegisterStageInput( const SDL_Scancode &_scancode ) noexcept {
    Input input;
    input.registered_vector = &InputsData.stage_inputs;
    input.input_index = InputsData.stage_inputs.size();
    InputsData.stage_inputs.emplace_back( _Input{
        .scancode = _scancode,
        .set = 0u,
    } );
    return input;
}
UNITS_NODISCARD Units::Input Units::InputsManager::RegisterSubStageInput( const SDL_Scancode &_scancode ) noexcept {
    Input input;
    input.registered_vector = &InputsData.substage_inputs;
    input.input_index = InputsData.substage_inputs.size();
    InputsData.substage_inputs.emplace_back( _Input{
        .scancode = _scancode,
        .set = 0u,
    } );
    return input;
}

static inline void SetInput( Units::_Input &_input_c ) noexcept {
    const bool raw   = _input_c.set[0u];
    const bool state = _input_c.set[InputsData::InputData_STATE];
    _input_c.set.set( InputsData::InputData_DOWN,  raw && !state );
    _input_c.set.set( InputsData::InputData_UP  , !raw &&  state );
    _input_c.set.set( InputsData::InputData_STATE, raw );
}
static inline void SetMouseButton( Units::Bitset<uint8_t> &_set ) noexcept {
	const bool raw   = _set[0u];
	const bool state = _set[InputsData::InputData_STATE];
    _set.set( InputsData::InputData_DOWN,  raw && !state );
    _set.set( InputsData::InputData_UP  , !raw &&  state );
    _set.set( InputsData::InputData_STATE, raw );
}

/* void Units::App::PollEvents() {
	Mouse.xrel = 0.0f;
	Mouse.yrel = 0.0f;
	Units::Mouse.scrollx = 0.0f;
	Units::Mouse.scrolly = 0.0f;
	while( SDL_PollEvent( &InputsData.event ) ) {
        // ImGui_ImplSDL3_ProcessEvent( &InputsData.event );
		if( InputsData.event.type == SDL_EVENT_QUIT ) {
			Units::App::Exit();
			break;
		}
		if( InputsData.event.type == SDL_EVENT_WINDOW_CLOSE_REQUESTED )
			DestroyWindowByID( InputsData.event.window.windowID );
		if( InputsData.event.type == SDL_EVENT_MOUSE_MOTION ) {
			Mouse.xrel = InputsData.event.motion.xrel;
			Mouse.yrel = InputsData.event.motion.yrel;
		}
		if( InputsData.event.type == SDL_EVENT_MOUSE_WHEEL ) {
			Units::Mouse.scrollx = InputsData.event.wheel.x;
			Units::Mouse.scrolly = InputsData.event.wheel.y;
		}

		/* Mouse buttons * {
			if( InputsData.event.type == SDL_EVENT_MOUSE_BUTTON_UP   ) {
				if( InputsData.event.button.button == SDL_BUTTON_LEFT )
					Units::_Mouse::_l.set( 0u, false );
				if( InputsData.event.button.button == SDL_BUTTON_MIDDLE )
					Units::_Mouse::_m.set( 0u, false );
				if( InputsData.event.button.button == SDL_BUTTON_RIGHT )
					Units::_Mouse::_r.set( 0u, false );
			}
			if( InputsData.event.type == SDL_EVENT_MOUSE_BUTTON_DOWN ) {
				if( InputsData.event.button.button == SDL_BUTTON_LEFT )
					Units::_Mouse::_l.set( 0u, true );
				if( InputsData.event.button.button == SDL_BUTTON_MIDDLE )
					Units::_Mouse::_m.set( 0u, true );
				if( InputsData.event.button.button == SDL_BUTTON_RIGHT )
					Units::_Mouse::_r.set( 0u, true );
			}
		}
		/* Inputs * {
			uint16_t lowest_count = SDL_min( InputsData.internal_inputs.size(), SDL_min( InputsData.stage_inputs.size(), InputsData.substage_inputs.size() ) );
			for( uint16_t i = 0u; i < lowest_count; i++ ) {
				if( InputsData.event.type == SDL_EVENT_KEY_UP ) {
					if( InputsData.event.key.scancode == (SDL_Scancode)InputsData.internal_inputs[i].scancode )
						InputsData.internal_inputs[i].set.set( 0u, false );
					if( InputsData.event.key.scancode ==    (SDL_Scancode)InputsData.stage_inputs[i].scancode )
						InputsData.stage_inputs[i].set.set( 0u, false );
					if( InputsData.event.key.scancode == (SDL_Scancode)InputsData.substage_inputs[i].scancode )
						InputsData.substage_inputs[i].set.set( 0u, false );
				}
				if( InputsData.event.type == SDL_EVENT_KEY_DOWN ) {
					if( InputsData.event.key.scancode == (SDL_Scancode)InputsData.internal_inputs[i].scancode )
						InputsData.internal_inputs[i].set.set( 0u,  true );
					if( InputsData.event.key.scancode ==    (SDL_Scancode)InputsData.stage_inputs[i].scancode )
						InputsData.stage_inputs[i].set.set( 0u,  true );
					if( InputsData.event.key.scancode == (SDL_Scancode)InputsData.substage_inputs[i].scancode )
						InputsData.substage_inputs[i].set.set( 0u,  true );
				}
			}
			for( uint16_t i = lowest_count; i < InputsData.internal_inputs.size(); i++ ) {
				if( InputsData.event.type == SDL_EVENT_KEY_UP   && InputsData.event.key.scancode == (SDL_Scancode)InputsData.internal_inputs[i].scancode )
					InputsData.internal_inputs[i].set.set( 0u, false );
				if( InputsData.event.type == SDL_EVENT_KEY_DOWN && InputsData.event.key.scancode == (SDL_Scancode)InputsData.internal_inputs[i].scancode )
					InputsData.internal_inputs[i].set.set( 0u,  true );
			}
			for( uint16_t i = lowest_count; i <    InputsData.stage_inputs.size(); i++ ) {
				if( InputsData.event.type == SDL_EVENT_KEY_UP   && InputsData.event.key.scancode ==    (SDL_Scancode)InputsData.stage_inputs[i].scancode )
					InputsData.stage_inputs[i].set.set( 0u, false );
				if( InputsData.event.type == SDL_EVENT_KEY_DOWN && InputsData.event.key.scancode ==    (SDL_Scancode)InputsData.stage_inputs[i].scancode )
					InputsData.stage_inputs[i].set.set( 0u,  true );
			}
			for( uint16_t i = lowest_count; i < InputsData.substage_inputs.size(); i++ ) {
				if( InputsData.event.type == SDL_EVENT_KEY_UP   && InputsData.event.key.scancode == (SDL_Scancode)InputsData.substage_inputs[i].scancode )
					InputsData.substage_inputs[i].set.set( 0u, false );
				if( InputsData.event.type == SDL_EVENT_KEY_DOWN && InputsData.event.key.scancode == (SDL_Scancode)InputsData.substage_inputs[i].scancode )
					InputsData.substage_inputs[i].set.set( 0u,  true );
			}
		}
	}
} 

void Units::GetInputs() {
	SDL_GetMouseState( &Mouse.x_pos, &Mouse.y_pos );
	SetMouseButton( Units::_Mouse::_l );
	SetMouseButton( Units::_Mouse::_m );
	SetMouseButton( Units::_Mouse::_r );
	
    uint16_t lowest_count = SDL_min( InputsData.internal_inputs.size(), SDL_min( InputsData.stage_inputs.size(), InputsData.substage_inputs.size() ) );
    for( uint16_t i = 0u; i < lowest_count; i++ ) {
        SetInput( InputsData.internal_inputs[i] );
        SetInput( InputsData.   stage_inputs[i] );
        SetInput( InputsData.substage_inputs[i] );
    }
    for( uint16_t i = lowest_count; i < InputsData.internal_inputs.size(); i++ )
        SetInput( InputsData.internal_inputs[i] );
    for( uint16_t i = lowest_count; i < InputsData.   stage_inputs.size(); i++ )
        SetInput( InputsData.   stage_inputs[i] );
    for( uint16_t i = lowest_count; i < InputsData.substage_inputs.size(); i++ )
        SetInput( InputsData.substage_inputs[i] );
} */