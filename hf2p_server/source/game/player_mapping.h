#pragma once
#include <memory\data.h>
#include <cseries\cseries.h>

struct s_player_mapping
{
	short active_input_user_count;
	short active_input_controller_count;
	ulong input_user_player_mapping[4];
	datum_index input_user_unit_mapping[4];
	ulong input_controller_player_mapping[4];
	ulong player_input_controller_mapping[16];
	ulong player_input_user_mapping[16];
	ulong active_output_user_count;
	ulong output_user_player_mapping[4];
	datum_index output_user_unit_mapping[4];
	byte player_output_user_mapping[16];
};

long __fastcall player_mapping_get_next_output_user(short absolute_player_index, long user_index); // e_output_user_index