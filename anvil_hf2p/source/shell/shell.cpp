#include "shell.h"
#include "cseries\cseries.h"

REFERENCE_DECLARE(0x1038280, bool, shell_application_paused);

#define SHELL_VERSION VERSION_PROJECT_NAME " " VERSION_

e_shell_application_type shell_application_type()
{
	return _shell_application_game;
}

e_shell_tool_type shell_tool_type()
{
	return _shell_tool_invalid;
}

bool shell_application_is_paused()
{
	return shell_application_paused;
}

const char* shell_get_version()
{
	return nullptr;
}
