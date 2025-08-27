#pragma once

#include "./units_macros.h"
#include "./units_types.h"
#include "./units_math.h"
// #include "./units_str.h"
#include "./units_stage.h"
#include "./units_inputs.h"

/* ### \- Global application initialization:
	Used for anything the entire applicaiton will
	use regardless of the current stage */
extern void Init();
/* ### \- Global applicaiton exit
	Used for exiting anything the application
	initialized during the global init */
extern void Exit();