// we delete build_info.obj every pre-build so the build date & time are forced to regenerate
#include "build_version.h"
#include <game\game.h>

#define PROJECT_NAME "Anvil Station"

#if defined(GAME_IS_DEDICATED_SERVER)
#define BUILD_CONFIG "Dedicated Server"
#else
#define BUILD_CONFIG "Game Server"
//#define BUILD_CONFIG "Game Client"
#endif

#ifdef _DEBUG
#define BUILD_TYPE "Debug"
#else
#define BUILD_TYPE "Release"
#endif

const char* anvil_build_version_string()
{
	return BUILD_TYPE " " __DATE__ " " __TIME__;
}

const char* anvil_get_config_string()
{
	// TODO "Game Client" once the client build config is added
	return BUILD_CONFIG;
}

const char* anvil_get_project_name_string()
{
	return PROJECT_NAME;
}

const char* anvil_get_build_name_string()
{
	return PROJECT_NAME " " BUILD_CONFIG;
}