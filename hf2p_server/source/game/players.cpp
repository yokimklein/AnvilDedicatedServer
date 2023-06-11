#include "players.h"
#include <stdio.h>
#include "..\memory\tls.h"
#include "game.h"
#include "..\simulation\game_interface\simulation_game_objects.h"

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

long player_mapping_get_input_user(word player_index)
{
	if (player_index == -1)
		return -1;
	else
		return get_tls()->player_mapping_globals->player_input_user_mapping[player_index];
}

void player_set_facing(datum_index player_index, real_vector3d* forward)
{
	s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
	if (game_is_authoritative())
	{
		if (player_data->unit_index != -1)
		{
			s_object_header* unit_object_header = (s_object_header*)datum_get(get_tls()->object_headers, player_data->unit_index);
			s_unit_data* unit_data = (s_unit_data*)unit_object_header->data;
			unit_data->facing_vector = *forward;
			unit_data->aiming_vector = *forward;
			unit_data->looking_vector = *forward;
			simulation_action_object_update(player_data->unit_index, _simulation_biped_update_desired_aiming_vector);
		}
	}
	long input_user_index = player_mapping_get_input_user((word)player_index);
	if (input_user_index != -1)
		player_control_set_facing(input_user_index, forward);
}

void player_control_set_facing(long input_user_index, real_vector3d* forward)
{
	FUNCTION_DEF(0x106780, void, __fastcall, player_control_set_facing_call, long input_user_index, real_vector3d* forward);
	player_control_set_facing_call(input_user_index, forward);
}

long player_index_from_absolute_player_index(short absolute_player_index)
{
	return datum_absolute_index_to_index(get_tls()->players, absolute_player_index);
}

void __fastcall player_increment_control_context(datum_index player_index)
{
	s_player_datum* player_data = (s_player_datum*)datum_get(get_tls()->players, player_index);
	datum_index unit_index = player_data->unit_index;
	if (unit_index != -1)
	{
		s_unit_data* unit = (s_unit_data*)datum_get(get_tls()->object_headers, unit_index);
		unit->control_context_identifier = player_data->control_context_identifier;
		simulation_action_object_update(unit_index, _simulation_biped_update_control_context);
	}
	player_data->control_context_identifier = (player_data->control_context_identifier + 1) & 0xF;
}