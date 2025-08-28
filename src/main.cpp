#include "units/units.h"
#define UNITS_ENTRY_POINT
#include "units/units_entry.h"

struct MainStage : Units::Stage_t {
} static MainStage{};

void Init() {
	Units::StageManager::LoadNextStage( &MainStage );
}
void Exit() {
}
