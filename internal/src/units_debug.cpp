#include <imgui/imgui.h>
#include <SDL3/SDL.h>
#include <stdint.h>
#include "units/units_macros.h"
#include "units/units_types.h"
#include "units/units_debug.h"

struct DebugData {
    const char* colors[9u] = {
        "\033[1;32m",
        "\033[1;37m",
        "\033[1;33m",
        "\033[1;31m",
        "\033[1;30m",
        "\033[1;34m",
        "\033[1;35m",
        "\033[1;36m",
        "\033[1;0m",
    };
    const char* levels[4u] = {
		" [passed]: ",
		"   [note]: ",
		"[warning]: ",
		"  [error]: ",
    };

	struct ConsoleApp {
		char input_buffer[256u];
		ImVector<char*> items;
		ImVector<const char*> commands;
		ImVector<char*> history;
		int32_t history_i;
		ImGuiTextFilter filter;
		bool auto_scroll;
		bool scroll_to_bottom;
		DebugData* debug_data_ptr;

		ConsoleApp( DebugData* _debug_data_ptr ): debug_data_ptr( _debug_data_ptr ) {
			clear_log();
			memset( input_buffer, 0, sizeof( input_buffer ) );
			history_i = -1;

			commands.push_back( "help" );
			commands.push_back( "clear" );
			commands.push_back( "history" );
			commands.push_back( "note" );

			auto_scroll = true;
			scroll_to_bottom = false;
		}
		~ConsoleApp() {
			clear_log();
			for( int i = 0; i < history.Size; i++ )
				ImGui::MemFree( history[i] );
		}
		
		static int   stricmp( const char *s1, const char *s2 )         { int d; while( ( d = toupper( *s2 ) - toupper( *s1 ) ) == 0 && *s1 ) { s1++; s2++; } return d; }
		static int   strnicmp( const char *s1, const char *s2, int n ) { int d = 0; while( n > 0 && ( d = toupper( *s2 ) - toupper( *s1 ) ) == 0 && *s1 ) { s1++; s2++; n--; } return d; }
		static char *strdup( const char *s )                           { IM_ASSERT(s); size_t len = strlen( s ) + 1; void *buf = ImGui::MemAlloc( len ); IM_ASSERT(buf); return (char*)memcpy( buf, (const void*)s, len ); }
		static void  strtrim( char *s )                                { char *str_end = s + strlen( s ); while( str_end > s && str_end[-1] == ' ' ) str_end--; *str_end = 0; }

		void clear_log() {
			for( uint32_t i = 0u; i < items.size(); i++ )
				ImGui::MemFree( items[i] );
			items.clear();
		}
		void add_log( const char *fmt, ... ) UNITS_VARIADIC_ARGS(2) {
			char buffer[256u];
			va_list args;
			va_start( args, fmt );
			vsnprintf( buffer, UNITS_ARRAYSIZE( buffer ), fmt, args );
			buffer[UNITS_ARRAYSIZE(buffer) - 1u] = 0;
			va_end( args );
			items.push_back( strdup( buffer ) );
		}
		void show( const char *title, bool *open_ptr ) {
			ImGui::SetNextWindowSize( ImVec2{ 520.0f, 600.0f }, ImGuiCond_FirstUseEver );
			if( !ImGui::Begin( title, open_ptr ) ) {
				ImGui::End();
				return;
			}
			if( ImGui::SmallButton( "clear" ) ) {
				clear_log();
			}
			ImGui::SameLine();
			filter.Draw( "Filter (\"incl,-excl\") (\"error\")", 180 );
			ImGui::Separator();

			const float footer_height_to_reserve = ImGui::GetStyle().ItemSpacing.y + ImGui::GetFrameHeightWithSpacing();
			if( ImGui::BeginChild( "log", ImVec2{ 0.0f, -footer_height_to_reserve }, ImGuiChildFlags_NavFlattened, ImGuiWindowFlags_HorizontalScrollbar ) ) {
				if( ImGui::BeginPopupContextWindow() ) {
					if( ImGui::Selectable( "clear" ) )
						clear_log();
					ImGui::EndPopup();
				}

				ImGui::PushStyleVar( ImGuiStyleVar_ItemSpacing, ImVec2{ 4.0f, 1.0f } );
				for( const char *item : items ) {
					if( !filter.PassFilter( item ) )
						continue;
					
					ImVec4 color;
					bool has_color = false;
						if( strstr( item, debug_data_ptr->levels[Units::Debug::DebugLevel_PASSED] ) ) { 
						color = ImVec4( 0.25f, 1.0f, 0.25f, 1.0f );
						has_color = true;
					} else if( strstr( item, "[command]: " ) ) {
						color = ImVec4( 0.25f, 1.0f, 1.0f, 1.0f );
						has_color = true;
					} else if( strstr( item, debug_data_ptr->levels[Units::Debug::DebugLevel_NOTE] ) ) {
						color = ImVec4( 1.0f, 1.0f, 1.0f, 1.0f );
						has_color = true;
					} else if( strstr( item, debug_data_ptr->levels[Units::Debug::DebugLevel_WARNING] ) ) {
						color = ImVec4( 1.0f, 0.75f, 0.25f, 1.0f );
						has_color = true;
					} else if( strstr( item, debug_data_ptr->levels[Units::Debug::DebugLevel_ERROR] ) ) {
						color = ImVec4( 1.0f, 0.25f, 0.25f, 1.0f );
						has_color = true;
					}
					if( has_color )
						ImGui::PushStyleColor( ImGuiCol_Text, color );
					ImGui::TextUnformatted( item );
					if( has_color )
						ImGui::PopStyleColor();
				}

				if( scroll_to_bottom || ( auto_scroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY() ) )
					ImGui::SetScrollHereY( 1.0f );
				scroll_to_bottom = false;

				ImGui::PopStyleVar();
			}
			ImGui::EndChild();
			ImGui::Separator();

			bool reclaim_focus = false;
			ImGuiInputTextFlags input_text_flags = ImGuiInputTextFlags_EnterReturnsTrue | ImGuiInputTextFlags_EscapeClearsAll | ImGuiInputTextFlags_CallbackCompletion | ImGuiInputTextFlags_CallbackHistory;
			if (ImGui::InputText("Input", input_buffer, IM_ARRAYSIZE( input_buffer ), input_text_flags, &text_edit_callback_stub, (void*)this ) ) {
				char *str = input_buffer;
				strtrim( str );
				if( str[0] )
					execute_command( str );
				strcpy( str, "" );
				reclaim_focus = true;
			}

			ImGui::SetItemDefaultFocus();
			if( reclaim_focus )
				ImGui::SetKeyboardFocusHere( -1 );

			ImGui::End();
		}
		void execute_command( const char *command_line ) {
			history_i = -1;
			for( int i = history.Size - 1; i >= 0; i-- )
				if( stricmp( history[i], command_line ) == 0 ) {
					ImGui::MemFree( history[i] );
					history.erase( history.begin() + i );
					break;
				}
			history.push_back( strdup( command_line ) );

			// Process command
			bool command_failed = false;
			if( stricmp( command_line, "clear" ) == 0 ) {
				clear_log();
			} else if( stricmp( command_line, "help" ) == 0 ) {
				add_log( "%sCommands:", debug_data_ptr->levels[Units::Debug::DebugLevel_NOTE] );
				for( int i = 0; i < commands.Size; i++ )
					add_log( "- %s", commands[i] );
			} else if( stricmp( command_line, "history" ) == 0 ) {
				int first = history.Size - 10;
				for( int i = first > 0 ? first : 0; i < history.Size; i++ )
					add_log( "%3d: %s\n", i, history[i] );
			} else if( strnicmp( command_line, "note", 4 ) == 0 ) {
				// only meant to bypass loggin command
				command_failed = true;
				add_log( "%s%s", debug_data_ptr->levels[Units::Debug::DebugLevel_NOTE], command_line );
			} else {
				command_failed = true;
				add_log( "%sUnknown command: '%s'\n", debug_data_ptr->levels[Units::Debug::DebugLevel_WARNING], command_line );
			}
			if( !command_failed )
				add_log( "[command]: %s\n", command_line );
			scroll_to_bottom = true;
		}
		static int text_edit_callback_stub( ImGuiInputTextCallbackData *data ) {
			ConsoleApp *console = (ConsoleApp*)data->UserData;
			return console->text_edit_callback(data);
		}
		int text_edit_callback( ImGuiInputTextCallbackData *data ) {
			switch( data->EventFlag ) {
			case ImGuiInputTextFlags_CallbackCompletion: {
					const char *word_end = data->Buf + data->CursorPos;
					const char *word_start = word_end;
					while( word_start > data->Buf ) {
						const char c = word_start[-1];
						if( c == ' ' || c == '\t' || c == ',' || c == ';' )
							break;
						word_start--;
					}
					ImVector<const char*> candidates;
					for(int i = 0; i < commands.Size; i++ )
						if( strnicmp( commands[i], word_start, (int)( word_end - word_start ) ) == 0 )
							candidates.push_back(commands[i]);
					if( candidates.Size == 0 ) {
						add_log( "No match for \"%.*s\"!\n", (int)( word_end - word_start ), word_start );
					} else if( candidates.Size == 1 ) {
						data->DeleteChars( (int)( word_start - data->Buf ), (int)( word_end - word_start ) );
						data->InsertChars( data->CursorPos, candidates[0] );
						data->InsertChars( data->CursorPos, " " );
					} else {
						int match_len = (int)( word_end - word_start );
						for( ; ; ) {
							int c = 0;
							bool all_candidates_matches = true;
							for( int i = 0; i < candidates.Size && all_candidates_matches; i++ )
								if( i == 0 )
									c = toupper( candidates[i][match_len] );
								else if( c == 0 || c != toupper( candidates[i][match_len] ) )
									all_candidates_matches = false;
							if( !all_candidates_matches )
								break;
							match_len++;
						}
						if( match_len > 0 ) {
							data->DeleteChars( (int)( word_start - data->Buf ), (int)( word_end - word_start ) );
							data->InsertChars( data->CursorPos, candidates[0], candidates[0] + match_len );
						}
						add_log( "Possible matches:\n" );
						for( int i = 0; i < candidates.Size; i++ )
							add_log( "- %s\n", candidates[i] );
					}
					break;
				}
			case ImGuiInputTextFlags_CallbackHistory: {
					const int prev_history_pos = history_i;
					if( data->EventKey == ImGuiKey_UpArrow ) {
						if( history_i == -1 )
							history_i = history.Size - 1;
						else if( history_i > 0 )
							history_i--;
					} else if( data->EventKey == ImGuiKey_DownArrow ) {
						if( history_i != -1 )
							if( ++history_i >= history.Size )
								history_i = -1;
					}

					if( prev_history_pos != history_i ) {
						const char *history_str = ( history_i >= 0 ) ? history[history_i] : "";
						data->DeleteChars( 0, data->BufTextLen );
						data->InsertChars( 0, history_str );
					}
				}
			}
			return 0;
		}
	} ConsoleApp{ this };
} static DebugData{};
 void Units::Debug::ShowDebugLogConsole( bool *_open_ptr ) {
	DebugData.ConsoleApp.show( "Debug Console", _open_ptr );
}

void Units::Debug::Log( const char *str ) {
	Log( DebugLevel_NOTE, str );
}
void Units::Debug::Log( const DebugColor &color, const char *str ) {
	char buffer[256u];
	SDL_snprintf( buffer, UNITS_ARRAYSIZE(buffer), "%s: %s", DebugData.levels[DebugLevel_NOTE], str );
	// console_app.add_log( "%s", buffer );
	SDL_Log( "%s%s%s", DebugData.colors[color], buffer, DebugData.colors[DebugColor_DEFAULT] );
}
void Units::Debug::Log( const DebugLevel &level, const char *str ) {
	char buffer[256u];
	SDL_snprintf( buffer, UNITS_ARRAYSIZE(buffer), "%s: %s", DebugData.levels[level], str );
	// console_app.add_log( "%s", buffer );
	SDL_Log( "%s%s%s", DebugData.colors[level], buffer, DebugData.colors[DebugColor_DEFAULT] );
}