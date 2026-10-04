#include "player_mapping.h"
#include <memory\tls.h>

long __fastcall player_mapping_get_next_output_user(short absolute_player_index, long user_index)
{
	return INVOKE(ADDRESS_PLAYER_MAPPING_GET_NEXT_OUTPUT_USER, player_mapping_get_next_output_user, absolute_player_index, user_index);
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

long player_mapping_get_unit_by_output_user(long user_index)
{
	if (user_index == NONE)
	{
		return NONE;
	}
	else
	{
		TLS_DATA_GET_VALUE_REFERENCE(player_mapping_globals);
		return player_mapping_globals->output_user_unit_mapping[user_index];
	}
}

void __fastcall player_mapping_set_input_user(long player_index, long input_user_index)
{
	INVOKE(ADDRESS_PLAYER_MAPPING_SET_INPUT_USER, player_mapping_set_input_user, player_index, input_user_index);
}

void __fastcall player_mapping_set_input_controller(long player_index, long controller_index)
{
	INVOKE(ADDRESS_PLAYER_MAPPING_SET_INPUT_CONTROLLER, player_mapping_set_input_controller, player_index, controller_index);
}

void __fastcall player_mapping_attach_output_user(long input_user_index, long player_index)
{
	INVOKE(ADDRESS_PLAYER_MAPPING_ATTACH_OUTPUT_USER, player_mapping_attach_output_user, input_user_index, player_index);
}
