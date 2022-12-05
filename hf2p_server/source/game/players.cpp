#include "players.h"

bool player_identifier_is_valid(s_player_identifier const* identifier)
{
	if (identifier->data != 0)
		return true;
	else
		return false;
}

// FUNC TODO
const char* player_identifier_get_string(s_player_identifier const* identifier)
{
	return "(null)";
}