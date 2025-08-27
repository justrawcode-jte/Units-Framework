#include "units/units.h"

struct StageData {
	bool is_current_paused{ false };
	bool is_new_stage     { false };
	bool is_new_substage  { false };
	Units::Stage_t* current_stage_ptr{ nullptr };
	Units::Stage_t*    next_stage_ptr{ nullptr };
	Units::SubStage_t* current_substage_ptr{ nullptr };
	Units::SubStage_t*    next_substage_ptr{ nullptr };
} static StageData{};


void Units::StageManager::LoadNextStage( Stage_t* stage_ptr ) noexcept {
	if( StageData.is_new_stage = ( stage_ptr != StageData.current_stage_ptr ) ) {
		if( stage_ptr != StageData.next_stage_ptr && StageData.next_stage_ptr != nullptr )
			delete StageData.next_stage_ptr;
		StageData.next_stage_ptr = stage_ptr;
	}
}

UNITS_NODISCARD Units::Stage_t* Units::StageManager::AttemptGetNextStage() noexcept {
	if( StageData.is_new_stage ) {
		if( StageData.current_stage_ptr != nullptr )
			delete StageData.current_stage_ptr;
		StageData.current_stage_ptr = StageData.next_stage_ptr;
		StageData.next_stage_ptr = nullptr;
	}
	StageData.is_new_stage = false;
	return StageData.current_stage_ptr;
}

UNITS_NODISCARD Units::Stage_t* Units::StageManager::GetCurrentStage() noexcept {
	return StageData.current_stage_ptr;
}

UNITS_NODISCARD const bool& Units::StageManager::NewStage() noexcept {
	return StageData.is_new_stage;
}

UNITS_NODISCARD const bool& Units::StageManager::CurrentPaused() noexcept {
	return StageData.is_current_paused;
}

void Units::StageManager::ClearNextStage() noexcept {
	if( StageData.next_stage_ptr != nullptr )
		delete StageData.next_stage_ptr;
	StageData.next_stage_ptr = nullptr;
	StageData.is_new_stage = false;
}

void Units::StageManager::LoadNextSubStage( SubStage_t* substage_ptr ) noexcept {
	StageData.is_current_paused = true;
	if( StageData.is_new_substage = ( substage_ptr != StageData.current_substage_ptr ) ) {
		if( substage_ptr != StageData.next_substage_ptr && StageData.next_substage_ptr != nullptr )
			delete StageData.next_substage_ptr;
		StageData.next_substage_ptr = substage_ptr;
	}
}

UNITS_NODISCARD Units::SubStage_t* Units::StageManager::AttemptGetNextSubStage() noexcept {
	if( StageData.is_new_substage ) {
		if( StageData.current_substage_ptr != nullptr )
			delete StageData.current_substage_ptr;
		StageData.current_substage_ptr = StageData.next_substage_ptr;
		StageData.next_substage_ptr = nullptr;
	}
	StageData.is_new_substage = false;
	return StageData.current_substage_ptr;
}

UNITS_NODISCARD Units::SubStage_t *Units::StageManager::GetCurrentSubStage() noexcept {
	return StageData.current_substage_ptr;
}

UNITS_NODISCARD const bool& Units::StageManager::NewSubStage() noexcept {
	return StageData.is_new_substage;
}

void Units::StageManager::EndSubStage() noexcept {
	StageData.is_current_paused = false;
}

void Units::StageManager::ClearCurrentSubStage() noexcept {
	StageData.is_current_paused = false;
	if( StageData.current_substage_ptr != nullptr )
		delete StageData.current_substage_ptr;
	StageData.current_substage_ptr = nullptr;
}

void Units::StageManager::ClearNextSubStage() noexcept {
	if( StageData.next_substage_ptr != nullptr )
		delete StageData.next_substage_ptr;
	StageData.next_substage_ptr = nullptr;
	StageData.is_new_substage = false;
}
