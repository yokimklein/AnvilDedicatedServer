#include "players.h"
#include <stdio.h>

bool player_identifier_is_valid(s_player_identifier const* identifier)
{
	if (*(qword*)identifier != 0)
		return true;
	else
		return false;
}

const char* player_identifier_get_string(s_player_identifier const* identifier)
{
	static char identifier_str[0x18]{};
	for (long i = 0; i < sizeof(s_player_identifier); i++)
		sprintf_s(identifier_str + (i * 3), 4, "%02x%c", *((byte*)identifier + i), i == (sizeof(s_player_identifier) - 1) ? 0 : ':');
	return identifier_str;
}