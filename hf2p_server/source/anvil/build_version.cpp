// we delete build_info.obj every pre-build so the build date & time are forced to regenerate
#include "build_version.h"

#ifdef _DEBUG
#define BUILD_CONFIG "Debug"
#else
#define BUILD_CONFIG "Release"
#endif

const char* anvil_dedi_build_version()
{
	return BUILD_CONFIG " " __DATE__ " " __TIME__;
}