#include "player_mapping.h"
#include <memory\tls.h>

long __fastcall player_mapping_get_next_output_user(short absolute_player_index, long user_index)
{
	return INVOKE(0xE1F90, player_mapping_get_next_output_user, absolute_player_index, user_index);
}

long player_mapping_get_player_by_input_user(e_input_user_index input_user_index)
{
	TLS_DATA_GET_VALUE_REFERENCE(player_mapping_globals);
	
	if (input_user_index == k_input_user_none)
	{
		return NONE;
	}

	datum_index player_index = player_mapping_globals->input_user_player_mapping[input_user_index];
	if (player_index != NONE)
	{
		ASSERT(player_mapping_globals->player_input_user_mapping[DATUM_INDEX_TO_ABSOLUTE_INDEX(player_index)] == input_user_index);
	}

	return player_index;
}