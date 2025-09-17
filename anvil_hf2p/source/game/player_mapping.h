#pragma once
#include <memory\data.h>
#include <cseries\cseries.h>

enum
{
	k_number_of_users = 4,
};

enum e_input_user_index
{
	_input_user_index0 = 0,
	_input_user_index1,
	_input_user_index2,
	_input_user_index3,

	k_number_of_input_users,

	k_input_user_none = -1
};

enum e_output_user_index
{
	_output_user_index0 = 0,
	_output_user_index1,
	_output_user_index2,
	_output_user_index3,

	k_number_of_output_users,

	k_output_user_none = -1
};

struct s_player_mapping_globals
{
	short active_input_user_count;
	short active_input_controller_count;
	c_static_array<datum_index, k_number_of_input_users> input_user_player_mapping;
	c_static_array<datum_index, k_number_of_input_users> input_user_unit_mapping;
	c_static_array<long, k_number_of_input_users> input_controller_player_mapping;
	c_static_array<long, 16> player_input_controller_mapping;
	c_static_array<long, 16> player_input_user_mapping;
	short active_output_user_count;
	c_static_array<long, k_number_of_output_users> output_user_player_mapping;
	c_static_array<datum_index, k_number_of_output_users> output_user_unit_mapping;
	c_static_array<byte, 16> player_output_user_mapping;
};

long __fastcall player_mapping_get_next_output_user(short absolute_player_index, long user_index); // e_output_user_index
long player_mapping_get_player_by_input_user(e_input_user_index input_user_index);
long player_mapping_get_unit_by_output_user(long user_index);