#pragma once

/* Units tag [[nodiscard]] */
#define UNITS_NODISCARD [[nodiscard]]
/* Units macro for sizeof( array ) */
#define UNITS_ARRAYSIZE(ARR) ((unsigned int)(sizeof(ARR) / sizeof((ARR)[0u])))
/* Units macros for variadic arguments */
#define UNITS_VARIADIC_ARGS(FMT) __attribute__((format(printf, FMT, FMT + 1)))