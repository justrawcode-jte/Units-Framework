#pragma once
#include <cctype>
#include "./units_macros.h"
#include "./units_types.h"

namespace Units {
    namespace Debug {
		/* ### \- Units DebugLevel log decorator tag
			Color logged text according to debug level */
        enum DebugLevel : uint8_t {
            DebugLevel_PASSED  = 0u,
            DebugLevel_NOTE    = 1u,
            DebugLevel_WARNING = 2u,
            DebugLevel_ERROR   = 3u,
        };
		/* ### \- Units DebugLevel log decorator color
			Colors logged text */
        enum DebugColor : uint8_t {
            DebugColor_GREEN   = 0u,
            DebugColor_WHITE   = 1u,
            DebugColor_YELLOW  = 2u,
            DebugColor_RED     = 3u,
            DebugColor_BLACK   = 4u,
            DebugColor_BLUE    = 5u,
            DebugColor_MAGENTA = 6u,
            DebugColor_CYAN    = 7u,
            DebugColor_DEFAULT = 8u,
        };
		/* Shows debug terminal within application */
		extern void ShowDebugLogConsole( bool* open_ptr );
		/* Logs a simple string to debug console */
		extern void Log( const char* str );
		/* Logs a fmt string to debug console */
		template<typename... Args>
		inline void Log( const char* fmt, Args&&... args ) {
			char buffer[256u];
			snprintf( buffer, UNITS_ARRAYSIZE(buffer), fmt, args... );
			Log( buffer );
		}
		/* Logs a simple string to debug console with color decorator */
		extern void Log( const DebugColor& color, const char* str );
		/* Logs a fmt string to debug console with color decorator */
		template<typename... Args>
		inline void Log( const DebugColor& color, const char* fmt, Args&&... args ) {
			char buffer[256u];
			snprintf( buffer, UNITS_ARRAYSIZE(buffer), fmt, args... );
			Log( color, buffer );
		}
		/* Logs a simple string to debug console with level tag */
		extern void Log( const DebugLevel& level, const char* str );
		/* Logs a fmt string to debug console with level tag */
		template<typename... Args>
		inline void Log( const DebugLevel& level, const char* fmt, Args&&... args ) {
			char buffer[256u];
			snprintf( buffer, UNITS_ARRAYSIZE(buffer), fmt, args... );
			Log( level, buffer );
		}
    } // namespace Debug
} // namespace UnitsEngine
