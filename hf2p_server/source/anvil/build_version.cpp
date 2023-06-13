// we delete build_info.obj every pre-build so the build date & time are forced to regenerate
#include "build_version.h"
#include <game\game.h>

#define PROJECT_NAME "Anvil Station"
#define BUILD_CONFIG_DS "Dedicated Server"
#define BUILD_CONFIG_GS "Game Server"
#define BUILD_CONFIG_GC "Game Client"
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
	if (game_is_dedicated_server())
		return BUILD_CONFIG_DS;
	else
		return BUILD_CONFIG_GS;
}

const char* anvil_get_project_name_string()
{
	return PROJECT_NAME;
}