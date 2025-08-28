#include "units/units.h"

int main( int argc, char** argv );
int WinMain( int argc, char** argv ) { return main( argc, argv ); }
int main( int argc, char** argv ) {
	Units::Debug::Log( "Hello World!" );
	return 0;
}
