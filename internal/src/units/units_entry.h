#pragma once

#ifdef UNITS_ENTRY_POINT

int main( int argc, char** argv );
int WinMain( int argc, char** argv ) { return main( argc, argv ); }
int main( int argc, char** argv ) {
	Init();
	Units::App::Run();
	Exit();
	return 0;
}

#endif