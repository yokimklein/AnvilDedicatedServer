#pragma once
#include "..\cseries\cseries.h"

enum e_podium_action
{
	_podium_action_base,
	_podium_action_loop,
	_podium_action_transition_to_armed_unarmed,
	_podium_action_move_in_from_transition_in,
	_podium_action_move_in_from_transition_out_no_loops,
	_podium_action_move_in_from_transition_out_can_loop,
	_podium_action_move_loop,
	_podium_action_move_out,
	_podium_action_transition_to_default,

	k_podium_action_count
};

struct s_player_podium
{
	c_enum<e_podium_action, long, k_podium_action_count> current_action;
	long ticks;
	long player_index;
	long player_model_index;
	bool female;
	long loop_count;
	long object_index;
	long object_index2;
	long object_index3;
	long stance_index;
	long move_index;
};
static_assert(sizeof(s_player_podium) == 0x2C);

static long* g_player_podiums_count = (long*)(module_base + 0x4A2973C);
static s_player_podium* g_player_podiums = (s_player_podium*)(module_base + 0x4A29740);

void hf2p_player_podium_initialize(long podium_biped_index, long player_index);
const static auto hf2p_set_user_loadout = (void (*)(long player_object_index))(module_base + 0xFB6E0);
const auto hf2p_set_player_cooldown = (void*)(module_base + 0xC19E0);
const auto sub_2E7BE0 = (void*)(module_base + 0x2E7BE0);