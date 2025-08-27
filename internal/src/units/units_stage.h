#pragma once
#include "./units_macros.h"

namespace Units {
	/* ### \- Units Stage_t class:
		Referred to as scenes in game engines */
	class Stage_t {
	public:
		Stage_t() {}
		virtual ~Stage_t() {}

		/* Enables inputs for the Stage_t
			Called shortly after loading the Stage_t */
		virtual void EnableInputs() {}
		/* Used for initializing member variables
			Called once before the main loop */
		virtual void Enter() {}
		/* Used for pre tick functions
			Called at the very beggining of every tick */
		virtual void BeginTick() {}
		/* Used for updating the Stage_t
			Called at fixed intervals
			- Cannot get input up or down accurately */
		virtual void Tick() {}
		/* Used mainly for updating UI
			Called after Tick in a non fixed interval 
			- Will get input up or down accurately */
		virtual void UITick() {}
		/* Used for rendering the Stage_t
			Called shortely after UITick */
		virtual void RenderPass() {}
		/* Used for post tick funcitons
			Called at the very end of every Tick */
		virtual void EndTick() {}
		/* Used to pause and load a SubStage_t
			Called shortly after the RenderPass */
		virtual void Pause() {}
		/* Used to call functions immediately after SubStage_t ends
			Called after SubStage_t ends */
		virtual void Unpause() {}
		/* Used to exit any member variables
			Called right before the Stage_t is deleted */
		virtual void Exit() {}
	};
	/* ### \- Units SubStage_t class:
		A special varient of Stage_t that pauses the
		current stage but keeps it rendered.
		Can be used for UI menus without switching the main
		stage and while keeping it rendered in the background. */
	class SubStage_t {
	public:
		SubStage_t() {}
		virtual ~SubStage_t() {}

		/* Enables inputs for the SubStage_t
			Called shortly after loading the SubStage_t */
		virtual void EnableInputs() {}
		/* Used for initializing member variables
			Called once before the sub loop */
		virtual void Enter() {}
		/* Used for pre tick functions
			Called at the very beggining of every tick */
		virtual void BeginTick() {}
		/* Used for rendering the Stage_t
			The only Tick available to SubStage_t's */
		virtual void UITick() {}
		/* Used for rendering the SubStage_t
			Called shortely after UITick */
		virtual void RenderPass() {}
		/* Used for post tick funcitons
			Called at the very end of every Tick */
		virtual void EndTick() {}
		/* Used to exit any member variables
			Called right before the SubStage_t is deleted */
		virtual void Exit() {}
	};
	namespace StageManager {
		extern void LoadNextStage( Stage_t* stage_ptr ) noexcept;
		UNITS_NODISCARD extern Stage_t* GetCurrentStage() noexcept;
		UNITS_NODISCARD extern Stage_t* AttemptGetNextStage() noexcept;
		UNITS_NODISCARD extern const bool& NewStage() noexcept;
		UNITS_NODISCARD extern const bool& CurrentPaused() noexcept;
		extern void ClearNextStage() noexcept;

		extern void LoadNextSubStage( SubStage_t* stage_ptr ) noexcept;
		UNITS_NODISCARD extern SubStage_t* GetCurrentSubStage() noexcept;
		UNITS_NODISCARD extern SubStage_t* AttemptGetNextSubStage() noexcept;
		UNITS_NODISCARD extern const bool& NewSubStage() noexcept;
		extern void EndSubStage() noexcept;
		extern void ClearCurrentSubStage() noexcept;
		extern void ClearNextSubStage() noexcept;
	} // namespace StageManager
} // namespace Core
