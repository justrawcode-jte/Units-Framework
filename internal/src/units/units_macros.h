#pragma once

/* Units tag [[nodiscard]] */
#define UNITS_NODISCARD [[nodiscard]]
/* Units macro for sizeof( array ) */
#define UNITS_ARRAYSIZE(ARR) ((unsigned int)(sizeof(ARR) / sizeof((ARR)[0u])))