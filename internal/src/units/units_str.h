#pragma once
#include <string>
#include <stdio.h>
#include "./units_macros.h"
#include "./units_types.h"

namespace Units {
	namespace str {
		inline void copy( char* dst, const char* src, const size_t& bytes ) {
			delete dst;
			dst = new char[bytes];
			strncpy( dst, src, bytes );
		}
		inline char* copy( const char* src ) {
			char* dst = nullptr;
			dst = new char[strlen( src ) + 1u];
			strncpy( dst, src, strlen( src ) + 1u );
			return dst;
		}
		template<typename... Args>
		inline void fmt( char* dst, const size_t& bytes, const char* fmt, Args&&... args ) {
			delete dst;
			dst = new char[bytes];
			snprintf( dst, bytes, fmt, args... );
		}
		template<typename... Args>
		inline char* fmt( const size_t& bytes, const char* fmt, Args&&... args ) {
			char* dst = nullptr;
			dst = new char[bytes];
			snprintf( dst, bytes, fmt, args... );
			return dst;
		}
	} // namespace Str
} // namespace UnitsEngine
