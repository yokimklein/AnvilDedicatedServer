// we delete build_info.obj every pre-build so the build date & time are forced to regenerate
#include "build_version.h"
#include <game\game.h>

const char* anvil_build_version_string()
{
	return TARGET_PROJECT_NAME " " TARGET_VARIANT " " TARGET_CONFIGURATION " " TARGET_PLATFORM " " TARGET_VERSION " " __DATE__ " " __TIME__;
}

const char* anvil_get_config_string()
{
#if defined(DEDICATED_SERVER_ENABLED)
	return "Dedicated Server";
#elif defined(GAME_CLIENT_ENABLED)
	return "Game Client";
#else
#error No window config string set!
#endif
}

const char* anvil_get_build_name_string()
{
	return TARGET_PROJECT_NAME;
}