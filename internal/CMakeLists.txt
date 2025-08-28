cmake_minimum_required( VERSION 3.14 )
project( Units_Framework LANGUAGES CXX VERSION 1.0.0 )
set( PROJECT_VERSION_STRING "\"${CMAKE_PROJECT_VERSION_MAJOR}.${CMAKE_PROJECT_VERSION_MINOR}.${CMAKE_PROJECT_VERSION_PATCH}\"" )

set( CMAKE_CXX_STANDARD 17 )
set( CMAKE_CXX_STANDARD_REQUIRED true )
set( CMAKE_CXX_EXTENSIONS on )

if( APPLE )
	set( CMAKE_OSX_SYSROOT /Library/Developer/CommandLineTools/SDKs/MacOSX.sdk )
endif()

if( CMAKE_BUILD_TYPE STREQUAL "Debug" )
	add_compile_definitions( -DAPP_BUILD_DEBUG )
elseif( CMAKE_BUILD_TYPE STREQUAL "Release" )
	add_compile_definitions( -DAPP_BUILD_RELEASE )
endif()

set( FRAMEWORK_DIR ${PROJECT_SOURCE_DIR} )
set( SOURCE_DIR ${PROJECT_SOURCE_DIR}/../ )

configure_file( ${FRAMEWORK_DIR}/src/units_config.h.in ${FRAMEWORK_DIR}/src/units/units_config.h )
configure_file( ${SOURCE_DIR}/src/config.h.in ${SOURCE_DIR}/src/config.h )

set( SDL3_DIR ${FRAMEWORK_DIR}/deps/sdl3/lib/cmake/SDL3 )
find_package( SDL3 REQUIRED )
set( SDL3_DIR ${FRAMEWORK_DIR}/deps/sdl3 )

set( ImGui_DIR ${FRAMEWORK_DIR}/deps/imgui )
add_subdirectory( ${FRAMEWORK_DIR}/deps/imgui )

file( GLOB_RECURSE CPP_FRAMEWORK_FILES ${FRAMEWORK_DIR}/src/*.cpp )
file( GLOB_RECURSE CPP_SOURCE_FILES ${SOURCE_DIR}/src/*.cpp )

add_executable( app ${CPP_FRAMEWORK_FILES} ${CPP_SOURCE_FILES} )

target_include_directories( app PRIVATE
	${SDL3_DIR}/include
	${ImGui_DIR}/include
	${FRAMEWORK_DIR}/src
	${SOURCE_DIR}/src
)
target_link_libraries( app PRIVATE
	SDL3::SDL3
	ImGui
)

install( TARGETS app DESTINATION bin )
install( DIRECTORY ${SOURCE_DIR}/assets/ DESTINATION assets PATTERN shaders EXCLUDE )
install( DIRECTORY ${SOURCE_DIR}/assets/shaders/compiled/ DESTINATION assets/shaders )

if( APPLE )
	set_target_properties( app PROPERTIES INSTALL_RPATH "@loader_path/../lib" )
	install( FILES
		${SDL3_DIR}/lib/libSDL3.0.dylib
	DESTINATION lib )
elseif( WIN32 )
	set_target_properties( app PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" )
	install( FILES $<TARGET_RUNTIME_DLLS:run> TYPE BIN )
endif()