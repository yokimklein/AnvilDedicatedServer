#pragma once
#include <cseries\cseries.h>
#include <game\game_engine_simulation.h>

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
	c_enum<e_podium_action, long, _podium_action_base, k_podium_action_count> current_action;
	long ticks;
	long player_index;
	long player_model_index;
	bool female;
	long loop_count;
	long player_unit_indices[3];
	long stance_index;
	long move_index;
};
static_assert(sizeof(s_player_podium) == 0x2C);

extern long& g_player_podium_count;
extern s_player_podium(&g_player_podiums)[k_maximum_multiplayer_players];

void __fastcall hf2p_player_podium_initialize(long podium_biped_index, long player_index);
void hf2p_trigger_player_podium_taunt(long player_podium_index);
void __fastcall hf2p_player_podium_increment_loop_count(long player_index);

//void hf2p_init_dedicated_server();