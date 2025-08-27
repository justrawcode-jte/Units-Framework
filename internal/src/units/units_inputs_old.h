#pragma once
#include <SDL3/SDL.h>
#include <vector>
#include "units_types.h"
#include "units_bit.h"

namespace Units {
    extern void InitInputs();
	extern void GetInputs();
    extern void DisableStageInputs() noexcept;
    extern void DisableSubStageInputs() noexcept;

	class Input;

	namespace InputsManager {
        UNITS_NODISCARD extern Input RegisterInternalInput( const SDL_Scancode &scancode ) noexcept;
        UNITS_NODISCARD extern Input RegisterStageInput( const SDL_Scancode &scancode ) noexcept;
        UNITS_NODISCARD extern Input RegisterSubStageInput( const SDL_Scancode &scancode ) noexcept;
	} // namespace InputsManager

	class Input{
    public:
        friend Input InputsManager::RegisterInternalInput( const SDL_Scancode &scancode ) noexcept;
        friend Input InputsManager::RegisterStageInput( const SDL_Scancode &scancode ) noexcept;
        friend Input InputsManager::RegisterSubStageInput( const SDL_Scancode &scancode ) noexcept;
    public:
        UNITS_NODISCARD const bool get_state() const noexcept {
            return (*registered_vector)[input_index].set.get( _InputData_STATE );
        }
        UNITS_NODISCARD const bool get_up() const noexcept {
            return (*registered_vector)[input_index].set.get( _InputData_UP );
        }
        UNITS_NODISCARD const bool get_down() const noexcept {
            return (*registered_vector)[input_index].set.get( _InputData_DOWN );
        }
    private:
        std::vector<_Input> *registered_vector;
        uint16_t input_index;
	};

	struct _Mouse {
		float32_t xrel{ 0.0f }, yrel{ 0.0f };
		float32_t x_pos{ 0.0f }, y_pos{ 0.0f };
		float32_t scrollx{ 0.0f }, scrolly{ 0.0f };
		UNITS_NODISCARD const bool get_sate( SDL_MouseButtonFlags button ) const noexcept {
			return button == SDL_BUTTON_LEFT ? _l.get( _InputData_STATE ) : ( button == SDL_BUTTON_MIDDLE ? _m.get( _InputData_STATE ) : ( button == SDL_BUTTON_RIGHT ? _r.get( _InputData_STATE ) : false ) );
		};
		UNITS_NODISCARD const bool get_up  ( SDL_MouseButtonFlags button ) const noexcept {
			return button == SDL_BUTTON_LEFT ? _l.get( _InputData_UP    ) : ( button == SDL_BUTTON_MIDDLE ? _m.get( _InputData_UP    ) : ( button == SDL_BUTTON_RIGHT ? _r.get( _InputData_UP    ) : false ) );
		};
		UNITS_NODISCARD const bool get_down( SDL_MouseButtonFlags button ) const noexcept {
			return button == SDL_BUTTON_LEFT ? _l.get( _InputData_DOWN  ) : ( button == SDL_BUTTON_MIDDLE ? _m.get( _InputData_DOWN  ) : ( button == SDL_BUTTON_RIGHT ? _r.get( _InputData_DOWN  ) : false ) );
		};
		static Units::Bitset<uint8_t> _l, _m, _r;
	} extern Mouse;
} // namespace Units
