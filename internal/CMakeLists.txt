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

configure_file( ${SOURCE_DIR}/src/config.h.in ${SOURCE_DIR}/src/config.h )

file( GLOB_RECURSE CPP_FRAMEWORK_FILES ${FRAMEWORK_DIR}/src/*.cpp )
file( GLOB_RECURSE CPP_SOURCE_FILES ${SOURCE_DIR}/src/*.cpp )

add_executable( app ${CPP_FRAMEWORK_FILES} ${CPP_SOURCE_FILES} )

target_include_directories( app PRIVATE
	${FRAMEWORK_DIR}/src
	${SOURCE_DIR}/src
)
target_link_directories( app PRIVATE
)

install( TARGETS app DESTINATION bin )
install( DIRECTORY ${SOURCE_DIR}/assets/ DESTINATION assets PATTERN shaders EXCLUDE )
install( DIRECTORY ${SOURCE_DIR}/assets/shaders/compiled/ DESTINATION assets/shaders )

if( APPLE )
	set_target_properties( app PROPERTIES INSTALL_RPATH "@loader_path/../lib" )
	install( FILES
	DESTINATION lib )
elseif( WIN32 )
	set_target_properties( app PROPERTIES INSTALL_RPATH "$ORIGIN/../lib" )
	install( FILES $<TARGET_RUNTIME_DLLS:run> TYPE BIN )
endif()