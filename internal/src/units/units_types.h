#pragma once
#include <stdint.h>
#include <limits>

/* Units float with fixed bit width
	Bit width: 32b */
typedef float       float32_t;
/* Units float with fixed bit width
	Bit width: 64b */
typedef double      float64_t;
/* Units float with fixed bit width
	Bit width: 128b */
typedef long double float128_t;


/* Units null values for uint's with fixed bit width
	Bit width: 8b */
const uint8_t  null8_v { std::numeric_limits<uint8_t >::max() };
/* Units null values for uint's with fixed bit width
	Bit width: 16b */
const uint16_t null16_v{ std::numeric_limits<uint16_t>::max() };
/* Units null values for uint's with fixed bit width
	Bit width: 32b */
const uint32_t null32_v{ std::numeric_limits<uint32_t>::max() };
/* Units null values for uint's with fixed bit width
	Bit width: 64b */
const uint64_t null64_v{ std::numeric_limits<uint64_t>::max() };