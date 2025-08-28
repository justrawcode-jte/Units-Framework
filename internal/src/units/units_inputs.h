#pragma once
#include <SDL3/SDL_events.h>
#include "./units_macros.h"
#include "./units_types.h"
#include "./units_bit.h"
#include "./units_app.h"


namespace Units {
	/* ### \- Units Input_t class:
		input class used for input handling */
	struct Input_t;

	namespace InputsManager {
		/* Initializes InputsManager */
		extern void Init() noexcept;

		/* Enables or Disables current Stage_t inputs */
		extern void EnableStageInputs   ( const bool& value = true ) noexcept;
		/* Enables or Disables current SubStage_t inputs */
		extern void EnableSubStageInputs( const bool& value = true ) noexcept;
		/* Returns true if current Stage_t inputs are enabled */
		UNITS_NODISCARD extern const bool&    StageInputsEnabled() noexcept;
		/* Returns true if current SubStage_t inputs are enabled */
		UNITS_NODISCARD extern const bool& SubStageInputsEnabled() noexcept;

		/* Clears global Input_t's from registry */
		extern void ClearGlobalInputs  () noexcept;
		/* Clears current Stage_t Input_t's from registry */
		extern void ClearStageInputs   () noexcept;
		/* Clears current SubStage_t Input_t's from registry */
		extern void ClearSubStageInputs() noexcept;

		/* Register an Input_t for use in global space */
		UNITS_NODISCARD extern Input_t RegisterGlobalInput  ( const SDL_Scancode& scancode ) noexcept;
		/* Register an Input_t for use in current Stage_t */
		UNITS_NODISCARD extern Input_t RegisterStageInput   ( const SDL_Scancode& scancode ) noexcept;
		/* Register an Input_t for use in current SubStage_t */
		UNITS_NODISCARD extern Input_t RegisterSubStageInput( const SDL_Scancode& scancode ) noexcept;

		/* Polls event raw info to update Input_t's
			- Call after events get polled, outside of the event loop */
		extern void PollInputs() noexcept;

		/* Get Input_t's current state */
		UNITS_NODISCARD extern const bool GetInputState( const Input_t& input ) noexcept;
		/* Returns true if Input_t was released */
		UNITS_NODISCARD extern const bool GetInputUp   ( const Input_t& input ) noexcept;
		/* Returns true if Input_t was pressed */
		UNITS_NODISCARD extern const bool GetInputDown ( const Input_t& input ) noexcept;

	} // namespace InputsManager

	struct Input_t {
	public:
		friend Input_t InputsManager::RegisterGlobalInput  ( const SDL_Scancode& scancode ) noexcept;
		friend Input_t InputsManager::RegisterStageInput   ( const SDL_Scancode& scancode ) noexcept;
		friend Input_t InputsManager::RegisterSubStageInput( const SDL_Scancode& scancode ) noexcept;

		friend const bool InputsManager::GetInputState( const Input_t& input ) noexcept;
		friend const bool InputsManager::GetInputUp   ( const Input_t& input ) noexcept;
		friend const bool InputsManager::GetInputDown ( const Input_t& input ) noexcept;

		friend void App::PollEvents();
	private:
		Input_t( const void* _registered_vector_ptr, const uint16_t& _input_id ) noexcept : registered_vector_ptr( _registered_vector_ptr ), input_id( _input_id ) {}
		const void* registered_vector_ptr;
		const uint16_t input_id;
	};
} // namespace Units
