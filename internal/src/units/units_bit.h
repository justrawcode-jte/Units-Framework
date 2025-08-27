#pragma once
#include <type_traits>
#include "./units_macros.h"
#include "./units_types.h"

namespace Units {
	template<typename Type_t>
	struct Bitset {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Type_t must be unsigned !!!" );
		Type_t bits;

		Bitset(): bits( 0u ) {}
		Bitset( const Type_t& val ): bits( val ) {}

		constexpr bool set( const uint8_t& i, const bool& val ) noexcept {
			const Type_t mask = 1u << i;
			bits = ( bits & ~mask ) | ( val ? mask : 0u );
			return val;
		}
		constexpr bool get( const uint8_t& i ) const noexcept {
			return bits & ( 1u << i );
		}

		UNITS_NODISCARD constexpr Type_t operator[]( const uint8_t& i ) noexcept {
			return bits & ( 1u << i );
		}

		UNITS_NODISCARD constexpr operator Type_t() noexcept {
			return bits;
		}

		UNITS_NODISCARD constexpr BitSet operator~() noexcept {
			return BitSet{~bits};
		}
	};


	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t popcount( Type_t num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		return num ? ( num & 1u ) + popcount<Type_t>( num >> 1u ) : 0u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t mostSignificantBit( Type_t num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		uint8_t msb = 0;
		while( num >>= 1 ) {
			msb += 1u;
		}
		return msb;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr bool hasSingleBit( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return false;
		return ( num & ( num - 1u ) ) == 0u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr bool isPowerOfTwo( const Type_t& num ) noexcept {
		return hasSingleBit<Type_t>( num );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t makeSingleBit( const uint8_t& i ) noexcept {
		return 1u << i;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t roundLogTwo( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return std::numeric_limits<Type_t>::digits;
		const uint8_t msb = mostSignificantBit<Type_t>( num );
		if( hasSingleBit<Type_t>( num ) )
			return msb;
		if( Bitset<Type_t>{num}[msb - 1u] )
			return msb + 1u;
		return msb;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t floorLogTwo( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return std::numeric_limits<Type_t>::digits;
		const uint8_t msb = mostSignificantBit<Type_t>( num );
		return msb;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t ceilLogTwo( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return std::numeric_limits<Type_t>::digits;
		const uint8_t msb = mostSignificantBit<Type_t>( num );
		if( hasSingleBit<Type_t>( num ) )
			return msb;
		return msb + 1u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t roundBaseTwo( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( roundLogTwo( num ) );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t floorBaseTwo( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( floorLogTwo( num ) );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t ceilBaseTwo( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( ceilLogTwo( num ) );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t roundBaseTwoFact( const Type_t& num ) noexcept {
		Type val = 0u;
		for( Type_t i = roundLogTwo<Type_t>( num ); i != std::numeric_limits<Type_t>::max; i-- )
			val += makeSingleBit<Type_t>( i );
		return val;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t floorBaseTwoFact( const Type_t& num ) noexcept {
		Type val = 0u;
		for( Type_t i = floorLogTwo<Type_t>( num ); i != std::numeric_limits<Type_t>::max; i-- )
			val += makeSingleBit<Type_t>( i );
		return val;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t ceilBaseTwoFact( const Type_t& num ) noexcept {
		Type val = 0u;
		for( Type_t i = ceilLogTwo<Type_t>( num ); i != std::numeric_limits<Type_t>::max; i-- )
			val += makeSingleBit<Type_t>( i );
		return val;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t roundLogFourBaseTwo( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return std::numeric_limits<Type_t>::digits;
		if( num == 1u )
			return 0u;
		const uint8_t msb = mostSignificantBit<Type_t>( num );
		if( hasSingleBit<Type_t>( num ) && !( msb & 1u ) )
			return msb;
		if( msb & 1u )
			return msb + 1u;
		if( BitSet<Type_t>{num}[msb - 1u] )
			return msb + 2u;
		return msb;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t floorLogFourBaseTwo( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return std::numeric_limits<Type_t>::digits;
		if( num == 1u )
			return 0u;
		const uint8_t msb = mostSignificantBit<Type_t>( num );
		if( hasSingleBit<Type_t>( num ) && !( msb & 1u ) )
			return msb;
		return msb >> 1u << 1u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr uint8_t ceilLogFourBaseTwo( const Type_t& num ) noexcept {
		static_assert( std::is_unsigned<Type_t>::value, "!!! Num must be unsigned !!!" );
		if( num == 0u )
			return std::numeric_limits<Type_t>::digits;
		if( num == 1u )
			return 0u;
		const uint8_t msb = mostSignificantBit<Type_t>( num );
		if( hasSingleBit<Type_t>( num ) && !( msb & 1u ) )
			return msb;
		if( msb & 1u )
			return msb + 1u;
		return msb + 2u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t roundLogFour( const Type_t& num ) noexcept {
		return roundLogFourBaseTwo( num ) >> 1u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t floorLogFour( const Type_t& num ) noexcept {
		return floorLogFourBaseTwo( num ) >> 1u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t ceilLogFour( const Type_t& num ) noexcept {
		return ceilLogFourBaseTwo( num ) >> 1u;
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t roundBaseFour( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( roundLogFourBaseTwo( num ) );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t floorBaseFour( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( floorLogFourBaseTwo( num ) );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t ceilBaseFour( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( ceilLogFourBaseTwo( num ) );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t roundSqrtBaseFour( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( roundLogFourBaseTwo( num ) >> 1u );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t floorSqrtBaseFour( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( floorLogFourBaseTwo( num ) >> 1u );
	}

	template<typename Type_t>
	UNITS_NODISCARD constexpr Type_t ceilSqrtBaseFour( const Type_t& num ) noexcept {
		return makeSingleBit<Type_t>( ceilLogFourBaseTwo( num ) >> 1u );
	}
} // namespace UnitsEngine