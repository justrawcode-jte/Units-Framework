#pragma once

#ifdef UNITS_ENTRY_POINT

int main( int argc, char** argv );
int WinMain( int argc, char** argv ) { return main( argc, argv ); }
int main( int argc, char** argv ) {
	Units::App::Init();
	Init();
	Units::App::Run();
	Exit();
	Units::App::Exit();
	return 0;
}

#endif